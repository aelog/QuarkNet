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


#ifndef INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_H
#define INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_H

#include <usrp_reader_gen2/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace usrp_reader_gen2 {

    /*!
     * \brief <+description of block+>
     * \ingroup usrp_reader_gen2
     *
     */
    class USRP_READER_GEN2_API rfid_reader_decoder : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<rfid_reader_decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of usrp_reader_gen2::rfid_reader_decoder.
       *
       * To avoid accidental use of raw pointers, usrp_reader_gen2::rfid_reader_decoder's
       * constructor is in a private implementation
       * class. usrp_reader_gen2::rfid_reader_decoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(float us_per_sample, float tari);
    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_READER_DECODER_H */

