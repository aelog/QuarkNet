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
#include "rfid_center_ff_impl.h"

namespace gr {
  namespace usrp_reader_gen2 {

    rfid_center_ff::sptr
    rfid_center_ff::make(int samples_per_pulse)
    {
      return gnuradio::get_initial_sptr
        (new rfid_center_ff_impl(samples_per_pulse));
    }

    /*
     * The private constructor
     */
    rfid_center_ff_impl::rfid_center_ff_impl(int samples_per_pulse)
      : gr::sync_block("rfid_center_ff",
                       gr::io_signature::make(1, 1, sizeof(float)),
                       gr::io_signature::make(1, 1, sizeof(float))),
        d_samples_per_pulse(samples_per_pulse)
    {
      int num_pulses = 32;  // Should be a large enough averaging window

      //Setup structure to hold samples. Used to track avg signal amplitude.

      d_window_length = num_pulses * samples_per_pulse;
      d_window_samples = (float *)malloc(d_window_length * sizeof(float));
      for (int i = 0; i < d_window_length; i++) {
        d_window_samples[i] = 0;
      }
      d_window_index = 0;
      d_avg_amp = 0;
    }

    /*
     * Our virtual destructor.
     */
    rfid_center_ff_impl::~rfid_center_ff_impl()
    {
    }

    int
    rfid_center_ff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      // Do <+signal processing+>

      for (int i = 0; i < noutput_items; i++) {

         //Track average amplitude
        d_avg_amp = ((d_avg_amp * (d_window_length - 1)) +
                     (d_avg_amp - d_window_samples[d_window_index]) +
                     std::abs(in[i])) / d_window_length;       //Calculate avg by factoring out oldest value, adding newest
        d_window_samples[d_window_index] = std::abs(in[i]);    //Replace oldest value
        d_window_index = (d_window_index + 1) % d_window_length; //Increment point to oldest value

        if (in[i] - d_avg_amp > 10) {
          out[i] = 10;
        }
        else if (in[i] - d_avg_amp < -10) {
          out[i] = -10;
        } else {
          out[i] = in[i] - d_avg_amp;
        }
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace usrp_reader_gen2 */
} /* namespace gr */

