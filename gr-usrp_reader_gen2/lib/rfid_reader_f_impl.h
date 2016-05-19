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

#ifndef INCLUDED_USRP_READER_GEN2_RFID_READER_F_IMPL_H
#define INCLUDED_USRP_READER_GEN2_RFID_READER_F_IMPL_H

#include <usrp_reader_gen2/rfid_reader_f.h>

namespace gr {
  namespace usrp_reader_gen2 {

    class rfid_reader_f_impl : public rfid_reader_f
    {
    private:

      int d_sample_rate;

      float *d_one;
      float *d_zero;
      float *d_query_cmd;
      float *d_reader_framesync;
      float *d_qrep;
      float *d_nak;
      float *d_req_rn;
      float *d_read;
      int d_one_len, d_zero_len, d_query_len, d_reader_framesync_len, d_qrep_len, d_nak_len, d_req_rn_len, d_read_len;

      float *cw_buffer;
      float *zero_buffer;
      float d_us_per_xmit;

      gr::message::sptr tx_msg;
      gr::message::sptr d_ctrl_msg;
      gr::msg_queue::sptr out_q;
      gr::msg_queue::sptr d_ctrl_q;
      int d_msg_count;

      float collision_threshold;
      float Q_fp;
      int d_num_empty_rounds;
      int d_tags_read_in_cycle, d_slots_occupied;

      char last_handle[16];
      int which_handle ;

      //Gen 2 parameters
      char d_CMD[5];
      char d_DR[2];
      char d_M[3];
      char d_tr_ext[2];
      char d_sel[3];
      char d_session[3];
      char d_target[2];
      char d_Q[5];
      char d_CRC[6];

      gr::msg_queue::sptr log_q;

      enum {
        LOG_START_CYCLE,
        LOG_QUERY,
        LOG_ACK,
        LOG_QREP,
        LOG_NAK,
        LOG_REQ_RN,
        LOG_READ,
        LOG_RN16,
        LOG_EPC,
        LOG_HANDLE,
        LOG_DATA,
        LOG_EMPTY,
        LOG_COLLISION,
        LOG_OKAY,
        LOG_ERROR
      };

      void gen_query_cmd();
      void gen_qrep_cmd();
      void gen_nak_cmd();
      void gen_req_rn_cmd();
      void gen_read_cmd(char * handle);

      void send_ack();
      void start_cycle();
      void send_query();
      void send_qrep();
      void send_nak();
      void send_req_rn();
      void send_read();

      bool send_another_query();
      void update_q(int slot_occupancy);

      int check_crc(char *bits, int num_bits);
      void set_num_samples_to_ungate();

      void log_msg(int message, char *text, int error);

    public:
      rfid_reader_f_impl(int sample_rate);
      ~rfid_reader_f_impl();

      // Where all the action really happens
      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);

      gr::msg_queue::sptr ctrl_q() const override;
      gr::msg_queue::sptr get_log() const override;

    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_READER_F_IMPL_H */

