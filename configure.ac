#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([animate], [1.0], [me@dyl.dog])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(build)
AC_CONFIG_MACRO_DIRS([m4])
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
AX_CHECK_COMPILE_FLAG([-std=gnu++1z],
    [# action-if-success
    AX_APPEND_FLAG([-std=gnu++1z], [CXX])
    ],
    [# action-if-fail
    AX_CHECK_COMPILE_FLAG([-std=c++1z],
        [# action-if-success
        AX_APPEND_FLAG([-std=c++1z], [CXX])
        ],
        [# action-if-fail
        AC_MSG_ERROR([c++1z support required.])
        ])
])

# Checks for libraries.
AC_CHECK_LIB([glfw], [glfwInit],,AC_MSG_ERROR([GLFW library required.]))
AC_CHECK_LIB([vulkan], [vkCreateInstance],,AC_MSG_ERROR([Vulkan library required.]))
AC_CHECK_LIB([pthread], [pthread_create],,AC_MSG_ERROR([pthread library required.]))
AC_CHECK_LIB([taquinsolve], [taquin_solve_c_stub],,AC_MSG_ERROR([taquinsolve library required.]))

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
                 src/Animation/Cat/Makefile
                 src/Animation/Cat/Object/Makefile
                 src/Animation/Makefile
                 src/Geometry/Makefile
                 src/Makefile
                 src/Object/Makefile
                 src/Object/Property/Makefile
                 src/VK/Makefile
                 test/Makefile])
AC_OUTPUT
