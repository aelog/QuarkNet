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

#ifndef INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_IMPL_H
#define INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_IMPL_H

#include <usrp_reader_gen2/rfid_command_gate_cc.h>

#ifndef READER_VARS
#include "rfid_global_vars.h"
#endif

namespace gr {
  namespace usrp_reader_gen2 {

    class rfid_command_gate_cc_impl : public rfid_command_gate_cc
    {
    private:
      float   d_us_per_rcv;
      int     d_pw;  //Reader pulsewidth in us
      int     d_T1;  //T1 value according to spec
      int     d_sample_rate;
      int     d_pass_count;
      int     d_pw_num_samples, d_T1_num_samples;
      double  d_max_rssi, d_min_rssi, d_avg_rssi, d_std_dev_rssi;
      int     d_sample_count;
      int     d_num_pulses;

      static constexpr float AVG_WIN = 1500; // Window to average amplitude over, in us
      static constexpr float THRESH_FRACTION = 0.75; //Percent of avg amplitude to detect edges
      static constexpr double MIN_AMP_THRESH = 0;     //Eventually, expose as user parameter
      float *d_window_samples;   //Array to hold samples for averaging amplitude
      int d_window_length;        //Length of window
      int d_window_index;         //Index to oldest sample
      double d_avg_amp;           //Average amplitude over window
      bool neg_edge_found;        //True if found negative edge for bit
      double d_thresh;            //Amplitude threshold for detecing edges
      double d_min_amp_thresh;    //To filter out nearby readers

      gr::msg_queue::sptr d_ctrl_out;  //Pipe control messages to reader block.

      bool is_negative_edge(float sample);
      bool is_positive_edge(float sample);
      void calc_signal_stats(float *buffer, int len, double *max, double *min, double *avg, double *std_dev);

    public:
      rfid_command_gate_cc_impl(int pw, int T1, int sample_rate);
      ~rfid_command_gate_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void set_ctrl_out(const gr::msg_queue::sptr msgq) { d_ctrl_out = msgq; }
    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_IMPL_H */

