AC_INIT([animate], [1.0], [me@dyl.dog])

AC_CONFIG_AUX_DIR(build)
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

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

AC_CHECK_LIB([glfw], [glfwInit],,AC_MSG_ERROR([GLFW library required.]))
AC_CHECK_LIB([vulkan], [vkCreateInstance],,AC_MSG_ERROR([Vulkan library required.]))
AC_CHECK_LIB([pthread], [pthread_create],,AC_MSG_ERROR([pthread library required.]))
AC_CHECK_LIB([taquinsolve], [taquin_solve_c_stub],,AC_MSG_ERROR([taquinsolve library required.]))

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/Animation/Makefile
 src/Animation/Cat/Makefile
 src/VK/Makefile
 src/Geometry/Makefile
 src/Object/Makefile
 src/Object/Property/Makefile
])
AC_OUTPUT
