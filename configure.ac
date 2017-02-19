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

AX_CHECK_COMPILE_FLAG([-lstdc++fs],
    [# action-if-success
    AX_APPEND_FLAG([-lstdc++fs], [CXX])
    ],
    [# action-if-fail
    AC_MSG_ERROR([stdc++fs support required.])
])


PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0 >= 3.8.0])
AM_PATH_GTK_3_0([3.20.0],[],AC_MSG_ERROR([Gtk+ 3.20.0 or higher required.]))

AC_CHECK_LIB([GL], [glBegin],,AC_MSG_ERROR([OpenGL library required.]))
AC_CHECK_LIB([GLU], [gluSphere],,AC_MSG_ERROR([GLU library required.]))
AC_CHECK_LIB([GLEW], [glewInit],,AC_MSG_ERROR([GLEW library required.]))
AC_CHECK_LIB([pthread], [pthread_create],,AC_MSG_ERROR([pthread library required.]))

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 Animations/Makefile
 GL/Makefile
])
AC_OUTPUT