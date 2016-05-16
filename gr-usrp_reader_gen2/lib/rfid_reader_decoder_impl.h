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

#ifndef INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_IMPL_H
#define INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_IMPL_H

#include <usrp_reader_gen2/rfid_reader_decoder.h>

namespace gr {
  namespace usrp_reader_gen2 {

    class rfid_reader_decoder_impl : public rfid_reader_decoder
    {
    private:

      enum {
        BEGIN,
        DELIM_FOUND,
        TARI_FOUND,
        RTCAL_FOUND,
        DATA
      };

      enum {
        READER_COMMAND,
        POWER_DOWN,
        START
      };

      static constexpr float AVG_WIN = 750; // Window to average amplitude over, in us
      static constexpr float THRESH_FRACTION = 0.9; //Percent of avg amplitude to detect edges
      static const int MAX_BITS = 256;
      static constexpr double MIN_AMP_THRESH = 0;     //Eventually, expose as user parameter

      double d_us_per_sample;
      int d_delim_width;          //Length of start delimiter, in samples
      int d_max_tari, d_min_tari, d_tari, d_rtcal, d_trcal; //Samples
      int d_state;                //Current state
      float * d_window_samples;   //Array to hold samples for averaging amplitude
      int d_window_length;        //Length of window
      int d_window_index;         //Index to oldest sample
      double d_avg_amp;           //Average amplitude over window
      double d_min_amp_thresh;    //To filter out nearby readers
      double d_thresh;            //Amplitude threshold for detecing edges
      int d_high_count, d_low_count, d_command_count, d_interarrival_count; //Sample counters
      bool neg_edge_found;        //True if found negative edge for bit
      int d_pivot;                // RTCal / 2. Determines data-0, data-1

      char d_bits[512];
      int d_len_bits;

      void advance_decoder(int next_state);
      bool is_negative_edge(float sample);
      bool is_positive_edge(float sample);
      void log_event(int event, int lag_samples);

    public:

      gr::msg_queue::sptr log_q;
      gr::msg_queue::sptr get_log() const { return log_q; }

      rfid_reader_decoder_impl(float us_per_sample, float tari);
      ~rfid_reader_decoder_impl();

      // Where all the action really happens
      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_IMPL_H */

