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

#ifndef INCLUDED_USRP_READER_GEN2_RFID_TAG_DECODER_F_IMPL_H
#define INCLUDED_USRP_READER_GEN2_RFID_TAG_DECODER_F_IMPL_H

#include <usrp_reader_gen2/rfid_tag_decoder_f.h>

namespace gr {
  namespace usrp_reader_gen2 {

    class rfid_tag_decoder_f_impl : public rfid_tag_decoder_f
    {
    private:
      //std::vector<float>	d_preamble_cor_vec;
      int d_preamble_offset;
      int d_one_vlen;
      char *d_tag_bit_vector;
      int d_skip_count;
      int d_samples_since_reset;
      int d_preamble_miss_threshold;
      float d_last_score;
      int d_samples_processed;

      gr::msg_queue::sptr d_ctrl_out;  //Pipe control messages to reader block.

    public:
      rfid_tag_decoder_f_impl();
      ~rfid_tag_decoder_f_impl();

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

#endif /* INCLUDED_USRP_READER_GEN2_RFID_TAG_DECODER_F_IMPL_H */

