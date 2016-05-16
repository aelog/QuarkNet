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


#ifndef INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_H
#define INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_H

#include <usrp_reader_gen2/api.h>
#include <gnuradio/block.h>
#include <gnuradio/message.h>
#include <gnuradio/msg_queue.h>

namespace gr {
  namespace usrp_reader_gen2 {

    /*!
     * \brief <+description of block+>
     * \ingroup usrp_reader_gen2
     *
     */
    class USRP_READER_GEN2_API rfid_command_gate_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<rfid_command_gate_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of usrp_reader_gen2::rfid_command_gate_cc.
       *
       * To avoid accidental use of raw pointers, usrp_reader_gen2::rfid_command_gate_cc's
       * constructor is in a private implementation
       * class. usrp_reader_gen2::rfid_command_gate_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int pw, int T1, int sample_rate);
    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_COMMAND_GATE_CC_H */

