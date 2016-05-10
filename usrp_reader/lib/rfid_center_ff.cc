/* -*- c++ -*- */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <rfid_center_ff.h>
#include <gr_io_signature.h>
#include <gri_mmse_fir_interpolator.h>
#include <stdexcept>
#include <float.h>
#include <string.h>
#include <cstdio>

#include <debug.h>
rfid_center_ff_sptr
rfid_make_center_ff(int samples_per_pulse)
{
  return rfid_center_ff_sptr(new rfid_center_ff(samples_per_pulse));
}

rfid_center_ff::rfid_center_ff(int samples_per_pulse)
  : gr_sync_block("rfid_center_ff",
		      gr_make_io_signature(1,1,sizeof(float)),
		      gr_make_io_signature(1,1,sizeof(float))),
    d_samples_per_pulse(samples_per_pulse)

{
  int num_pulses = 32;  // Should be a large enough averaging window

  //Setup structure to hold samples. Used to track avg signal amplitude.

  d_window_length = num_pulses * samples_per_pulse;
  d_window_samples = (float *)malloc(d_window_length * sizeof(float));
  for (int i = 0; i < d_window_length; i++){
    d_window_samples[i] = 0;
  }
  d_window_index = 0;
  d_avg_amp = 0;
  d_pass_count = 0;
  d_avg_amp_back = 0;
}

rfid_center_ff::~rfid_center_ff()
{
}

inline bool
rfid_center_ff::is_negative_edge(float sample){
  return sample < d_thresh_low;
}

inline bool
rfid_center_ff::is_positive_edge(float sample){
  return sample > d_thresh_high;
}

int
rfid_center_ff::work(int noutput_items,
		     gr_vector_const_void_star &input_items,
		     gr_vector_void_star &output_items)
{
  const float *in = (const float *) input_items[0];
  float* out = (float *) output_items[0];

  int start = 0;

  for(int i = 0; i < noutput_items; i++){

     //Track average amplitude
    d_avg_amp = ((d_avg_amp * (d_window_length - 1)) +
		 (d_avg_amp - d_window_samples[d_window_index]) +
		 std::abs(in[i])) / d_window_length;       //Calculate avg by factoring out oldest value, adding newest
    d_window_samples[d_window_index] = std::abs(in[i]);    //Replace oldest value
    d_window_index = (d_window_index + 1) % d_window_length; //Increment point to oldest value

    d_thresh_low = d_avg_amp * 0.995;
    d_thresh_high = d_avg_amp * 1.005;

    //if(is_negative_edge(std::abs(in[i])) && !neg_edge_found){
    if((is_positive_edge(std::abs(in[i])) || is_negative_edge(std::abs(in[i])) )&& !global_reader_state->center_edge_found){
	d_avg_amp_back = 0;
	amp_count = 0;
	global_reader_state->center_edge_found = true;
	global_reader_state->center_tracking = true;
	//start = i;
	for(int m=i; m<noutput_items; m++){
                d_avg_amp_back += std::abs(in[m]);
        }
        d_avg_amp_back = d_avg_amp_back/(noutput_items-i);
    }

    /*if(global_reader_state->center_edge_found && global_reader_state->center_tracking){
	d_avg_amp_back += std::abs(in[i]);
    }
    else if(global_reader_state->center_edge_found && !global_reader_state->center_tracking){
	out[i] = in[i] - d_avg_amp_back;
    }
    else{
	out[i] = in[i] - d_avg_amp;
    }*/

    if(global_reader_state->center_edge_found && global_reader_state->center_tracking){
	amp_count++;
	if(amp_count>=d_window_length){
		global_reader_state->center_tracking = false;
	}

	if(in[i] - d_avg_amp_back > 10){
		out[i] = 10;
	}
	else if(in[i] - d_avg_amp_back < -10){
		out[i] = -10;
	}
	else{
		//out[i] = in[i] - d_avg_amp_back;
		if(i<noutput_items-1){
			out[i] = (in[i] + in[i+1] - 2*d_avg_amp_back)/2;
		}
		else{
			out[i] = in[i] - d_avg_amp_back;
		}
	}
    }
    else{

	if(in[i] - d_avg_amp > 10){
		out[i] = 10;
	}
	else if(in[i] - d_avg_amp < -10){
		out[i] = -10;
	}
	else{
		//out[i] = in[i] - d_avg_amp;
		if(i<noutput_items-1){
                        out[i] = (in[i] + in[i+1] - 2*d_avg_amp)/2;
                }
                else{
                        out[i] = in[i] - d_avg_amp;
                }
	}
    }
  }

  /*if(global_reader_state->center_edge_found && global_reader_state->center_tracking){
	d_avg_amp_back = d_avg_amp_back/(noutput_items-start);
	for(int m=start; m<noutput_items; m++){
		out[m] = in[m] - d_avg_amp_back;
	}
	global_reader_state->center_tracking = false;
  }*/

  return noutput_items;
}


void
rfid_center_ff::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
  /*unsigned ninputs = ninput_items_required.size ();
  for (unsigned i = 0; i < ninputs; i++){
    ninput_items_required[i] = noutput_items + history();
  }*/
}
