#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([animate], [1.0], [me@dyl.dog])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for c++17 support
AX_CHECK_COMPILE_FLAG([-std=c++17],
    [# action-if-success
    AX_APPEND_FLAG([-std=c++17], [CXX])
    ],
    [# action-if-fail
    AC_MSG_ERROR([c++17 support required.])
])

# Checks for libraries.
AC_CHECK_LIB([glfw], [glfwInit],,AC_MSG_ERROR([GLFW library required.]))
AC_CHECK_LIB([vulkan], [vkCreateInstance],,AC_MSG_ERROR([Vulkan library required.]))
AC_CHECK_LIB([pthread], [pthread_create],,AC_MSG_ERROR([pthread library required.]))
AC_CHECK_LIB([taquinsolve], [taquin_solve_c_stub],,AC_MSG_ERROR([taquinsolve library required.]))
AC_CHECK_LIB([casspir], [casspir_c_stub],,AC_MSG_ERROR([casspir library required.]))

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday memset pow sqrt strtol])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
