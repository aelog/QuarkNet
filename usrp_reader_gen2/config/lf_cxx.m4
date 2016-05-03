dnl Autoconf support for C++
dnl Copyright (C) 1988 Eleftherios Gkioulekas <lf@amath.washington.edu>
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Boston, MA 02110-1301, USA.
dnl
dnl As a special exception to the GNU General Public License, if you
dnl distribute this file as part of a program that contains a configuration
dnl script generated by Autoconf, you may include it under the same
dnl distribution terms that you use for the rest of that program.

# -----------------------------------------------------------------
# This macro should be called to configure your C++ compiler.
# When called, the macro does the following things:
# 1. It finds an appropriate C++ compiler
#    If you passed the flag --with-cxx=foo, then it uses that
#    particular compiler
# 2. Checks whether the compiler accepts the -g
# ------------------------------------------------------------------

AC_DEFUN([LF_CONFIGURE_CXX],[
 AC_REQUIRE([AC_PROG_CXX])dnl
 AC_REQUIRE([AC_PROG_CXXCPP])dnl
 LF_CXX_PORTABILITY
])

# -----------------------------------------------------------------------
# This macro tests the C++ compiler for various portability problem.
# -----------------------------------------------------------------------


AC_DEFUN([LF_CXX_PORTABILITY],[

  dnl
  dnl Check for common C++ portability problems
  dnl

  dnl AC_LANG_PUSH
  dnl AC_LANG_CPLUSPLUS
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS


  dnl Test whether C++ has std::isnan
  AC_MSG_CHECKING(whether C++ has std::isnan)
  AC_TRY_COMPILE([#include <cmath>], [
   std::isnan(0);
], [ AC_MSG_RESULT(yes)
	AC_DEFINE(CXX_HAS_STD_ISNAN,[],[Define if has std::isnan]) ],
   [ AC_MSG_RESULT(no) ])

  dnl Done with the portability checks
  dnl AC_LANG_POP([C++])
  AC_LANG_RESTORE
])

