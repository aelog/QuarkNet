/* -*- c++ -*- */

#define USRP_READER_GEN2_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "usrp_reader_gen2_swig_doc.i"

%{
#include "usrp_reader_gen2/rfid_center_ff.h"
#include "usrp_reader_gen2/rfid_clock_recovery_zc_ff.h"
#include "usrp_reader_gen2/rfid_command_gate_cc.h"
%}


%include "usrp_reader_gen2/rfid_center_ff.h"
GR_SWIG_BLOCK_MAGIC2(usrp_reader_gen2, rfid_center_ff);
%include "usrp_reader_gen2/rfid_clock_recovery_zc_ff.h"
GR_SWIG_BLOCK_MAGIC2(usrp_reader_gen2, rfid_clock_recovery_zc_ff);
%include "usrp_reader_gen2/rfid_command_gate_cc.h"
GR_SWIG_BLOCK_MAGIC2(usrp_reader_gen2, rfid_command_gate_cc);
