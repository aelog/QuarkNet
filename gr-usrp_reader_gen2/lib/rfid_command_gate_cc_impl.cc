/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "rfid_command_gate_cc_impl.h"

namespace gr {
  namespace usrp_reader_gen2 {

    rfid_command_gate_cc::sptr
    rfid_command_gate_cc::make(int pw, int T1, int sample_rate)
    {
      return gnuradio::get_initial_sptr
        (new rfid_command_gate_cc_impl(pw, T1, sample_rate));
    }

    /*
     * The private constructor
     */
    rfid_command_gate_cc_impl::rfid_command_gate_cc_impl(int pw, int T1, int sample_rate)
      : gr::block("rfid_command_gate_cc",
                  gr::io_signature::make(1, 1, sizeof(gr_complex)),
                  gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_pw(pw),
        d_T1(T1),
        d_sample_rate(sample_rate)
    {
      d_pw_num_samples = (int)((d_pw / (float) 1000000) * d_sample_rate);

      d_T1_num_samples =  (int)((d_T1 / (float)1000000) * d_sample_rate);
      global_reader_state->T1_value = d_T1;
      global_reader_state->us_per_rcv = (float)1000000 / d_sample_rate;
      printf("us Per Sample: %f Num samples per pulse:%d T1:%d\n", global_reader_state->us_per_rcv, d_pw_num_samples, d_T1_num_samples);
      init_global_reader_state();

      //Setup structure to hold samples. Used to track avg signal amplitude.

      d_window_length = (int)((1 / (1000000 / (float)d_sample_rate)) * AVG_WIN);
      d_window_samples = (float *)malloc(d_window_length * sizeof(float));
      for (int i = 0; i < d_window_length; i++) {
        d_window_samples[i] = 0;
      }
      d_window_index = 0;
      d_avg_amp = 0;

      //Set up timer

      timeval t = {READER_CYCLE_TIMER_RATE / 1000, (READER_CYCLE_TIMER_RATE % 1000) * 1000};
      timer.it_interval = t;
      timer.it_value = t;

      signal(SIGALRM, catch_trigger_alarm);
      setitimer(ITIMER_REAL, &timer, NULL);
    }

    /*
     * Our virtual destructor.
     */
    rfid_command_gate_cc_impl::~rfid_command_gate_cc_impl()
    {
    }

    void
    rfid_command_gate_cc_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
      unsigned ninputs = ninput_items_required.size();
      for (unsigned i = 0; i < ninputs; i++) {
        ninput_items_required[i] = noutput_items + history();
      }
    }

    int
    rfid_command_gate_cc_impl::general_work(int noutput_items,
                                            gr_vector_int &ninput_items,
                                            gr_vector_const_void_star &input_items,
                                            gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>

      int consumed = 0;
      int written = 0;

      float flt_value = 0;

      //printf("cg\n");

      for (int i = 0; i < std::min(noutput_items , ninput_items[0]); i++) {

        if (global_reader_state->command_gate_status == GATE_RESET) {
          global_reader_state->command_gate_status = GATE_CLOSED;
          d_sample_count = 0;
        }

        //Track average amplitude
        d_avg_amp = ((d_avg_amp * (d_window_length - 1)) +
                     (d_avg_amp - d_window_samples[d_window_index]) +
                     std::abs(in[i])) / d_window_length;       //Calculate avg by factoring out oldest value, adding newest
        d_window_samples[d_window_index] = std::abs(in[i]);    //Replace oldest value
        d_window_index = (d_window_index + 1) % d_window_length; //Increment point to oldest value

        d_thresh = d_avg_amp * THRESH_FRACTION;  //Threshold for detecting negative/positive edges

        consumed++;
        flt_value = std::abs(in[i]);

        if (global_reader_state->command_gate_status == GATE_CLOSED) {

          d_sample_count++;

          if (is_negative_edge(std::abs(in[i])) && !neg_edge_found) {
            d_sample_count = 0;
            neg_edge_found = true;
          }

          if (neg_edge_found) {
            if (is_positive_edge(std::abs(in[i]))) {
              if (d_sample_count > d_pw_num_samples / 2) {
                d_num_pulses++;
                d_sample_count++;
                neg_edge_found = false;
              } else {
                //Too short to be a reader pulse
                d_num_pulses = 0;
              }
              d_sample_count = 0;
            }
          }

          if (d_sample_count == (d_T1_num_samples / 4) * 3) {
            //Calculate noise power
            int start_index = d_window_index - (d_T1_num_samples / 4) - 1; //Look back in buffer.
            float * buffer = (float *)malloc((d_T1_num_samples / 4) * sizeof(float));

            if (start_index < 0) {
              start_index = d_window_length + start_index;  //Counting in from the end.
            }

            for (int j = 0; j < d_T1_num_samples / 4; j++) {
              buffer[j] = std::abs(d_window_samples[(j + start_index) % d_window_length]);
            }

            calc_signal_stats(buffer,
                              d_T1_num_samples / 4,
                              &global_reader_state->max_pwr,
                              &global_reader_state->min_pwr,
                              &global_reader_state->avg_pwr,
                              &global_reader_state->std_dev_noise);
          }

          if (d_sample_count > d_T1_num_samples) {
            // > 5 avoids triggering on power down and TRCal
            if (!neg_edge_found && d_num_pulses > 5) {
              global_reader_state->command_gate_status = GATE_OPEN;
              global_reader_state->decoder_status = DECODER_SEEK_PREAMBLE;
            }
            d_num_pulses = 0;
            d_sample_count = 0;
          }
        }

        if (global_reader_state->command_gate_status == GATE_OPEN) {

          //Calculate signal power
          if (d_sample_count == d_T1_num_samples * 3) { //Should be well into signal.

            int start_index = d_window_index - (d_T1_num_samples / 4) - 1; //Look back in buffer.
            float * buffer = (float *)malloc((d_T1_num_samples / 4) * sizeof(float));

            if (start_index < 0) {
              start_index = d_window_length + start_index;  //Counting in from the end.
            }

            for (int j = 0; j < d_T1_num_samples / 4; j++) {
              buffer[j] = std::abs(d_window_samples[(j + start_index) % d_window_length]);
            }

            calc_signal_stats(buffer,
                              d_T1_num_samples / 4,
                              &global_reader_state->max_pwr,
                              &global_reader_state->min_pwr,
                              &global_reader_state->avg_pwr,
                              &global_reader_state->std_dev_signal);
          }

          //We need to just skip over the NAK. Flag sent by reader when NAK is sent.
          if (global_reader_state->nak_sent) {
            global_reader_state->nak_sent = false;
            global_reader_state->command_gate_status = GATE_RESET;
          } else {
            out[written++] = in[i];
            if (d_sample_count++ > global_reader_state->num_samples_to_ungate) {
              global_reader_state->command_gate_status = GATE_RESET;
            }
          }
        }
      }

      if (trigger_cycle) {
        // printf("Trigger cycle:%d\n", global_reader_state->cur_cycle);
        trigger_cycle = false;

        if (global_reader_state->cur_cycle < global_reader_state->num_cycles) {
          gr_message_sptr ctrl_msg = gr_make_message(0,
                                                     sizeof(int),
                                                     0,
                                                     (1) * sizeof(int));
          int command[] = {TIMER_FIRED};
          memcpy(ctrl_msg->msg(), &command, 1 * sizeof(int));
          d_ctrl_out->insert_tail(ctrl_msg);

          // NOTE: verbose output
//        printf("Timer fired starting cycle\n");
        }

        if (global_reader_state->cur_cycle == global_reader_state->num_cycles + 1) {
          printf("Last Cycle Started\n");
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each(consumed);

      // Tell runtime system how many output items we produced.
      return written;
    }


    inline bool
    rfid_command_gate_cc_impl::is_negative_edge(float sample)
    {
        return sample < d_thresh;
    }

    inline bool
    rfid_command_gate_cc_impl::is_positive_edge(float sample)
    {
        return sample > d_thresh;
    }

    /* How this function is called:
     *
     * 1. calc_signal_stats(buffer,
     * 			d_T1_num_samples / 4,		// len
     * 			&global_reader_state->max_pwr,
     * 			&global_reader_state->min_pwr,
     * 			&global_reader_state->avg_pwr,
     * 			&global_reader_state->std_dev_noise);
     *
     * 2. calc_signal_stats(buffer,
     * 			d_T1_num_samples / 4,		// len
     * 			&global_reader_state->max_pwr,
     * 			&global_reader_state->min_pwr,
     * 			&global_reader_state->avg_pwr,
     * 			&global_reader_state->std_dev_signal);
     */
    void
    rfid_command_gate_cc_impl::calc_signal_stats(float *buffer,
                                                 int len,
                                                 double *max,
                                                 double *min,
                                                 double *avg,
                                                 double *std_dev)
    {
      *max = DBL_MIN;
      *min = DBL_MAX;
      double tmp_avg = 0;
      double tmp_std_dev = 0;

      for (int i = 0; i < len; i++) {

        tmp_avg += buffer[i];

        if (buffer[i] > * max) {
          *max = buffer[i];
        }

        if (buffer[i] < * min) {
          *min = buffer[i];
        }
      }

      tmp_avg = tmp_avg / len;

      //Calculate STD_DEV
      for (int i = 0; i < len; i++) {
        tmp_std_dev += std::pow((buffer[i] - tmp_avg), 2);
      }

      tmp_std_dev = tmp_std_dev / len;
      tmp_std_dev = sqrt(tmp_std_dev);

      *avg = tmp_avg;
      *std_dev = tmp_std_dev;
    }

  } /* namespace usrp_reader_gen2 */
} /* namespace gr */
