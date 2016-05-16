# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND usrp_reader_gen2_sources
    rfid_center_ff_impl.cc
    rfid_clock_recovery_zc_ff_impl.cc
    rfid_command_gate_cc_impl.cc
    rfid_global_vars.cc
)

set(usrp_reader_gen2_sources "${usrp_reader_gen2_sources}" PARENT_SCOPE)
if(NOT usrp_reader_gen2_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT usrp_reader_gen2_sources)

add_library(gnuradio-usrp_reader_gen2 SHARED ${usrp_reader_gen2_sources})
target_link_libraries(gnuradio-usrp_reader_gen2 ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-usrp_reader_gen2 PROPERTIES DEFINE_SYMBOL "gnuradio_usrp_reader_gen2_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-usrp_reader_gen2 PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-usrp_reader_gen2
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_usrp_reader_gen2_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_usrp_reader_gen2.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_usrp_reader_gen2.cc
)

add_executable(test-usrp_reader_gen2 ${test_usrp_reader_gen2_sources})

target_link_libraries(
  test-usrp_reader_gen2
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-usrp_reader_gen2
)

GR_ADD_TEST(test_usrp_reader_gen2 test-usrp_reader_gen2)
