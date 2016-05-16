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


#ifndef INCLUDED_USRP_READER_GEN2_RFID_CLOCK_RECOVERY_ZC_FF_H
#define INCLUDED_USRP_READER_GEN2_RFID_CLOCK_RECOVERY_ZC_FF_H

#include <usrp_reader_gen2/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace usrp_reader_gen2 {

    /*!
     * \brief <+description of block+>
     * \ingroup usrp_reader_gen2
     *
     */
    class USRP_READER_GEN2_API rfid_clock_recovery_zc_ff : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<rfid_clock_recovery_zc_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of usrp_reader_gen2::rfid_clock_recovery_zc_ff.
       *
       * To avoid accidental use of raw pointers, usrp_reader_gen2::rfid_clock_recovery_zc_ff's
       * constructor is in a private implementation
       * class. usrp_reader_gen2::rfid_clock_recovery_zc_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samples_per_pulse, int interp_factor);
    };

  } // namespace usrp_reader_gen2
} // namespace gr

#endif /* INCLUDED_USRP_READER_GEN2_RFID_CLOCK_RECOVERY_ZC_FF_H */
