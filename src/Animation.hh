#pragma once

#include <gtk/gtk.h>

namespace Animate {

    class Animation {
        public:
            void on_render(GtkGLArea *gl_area, GdkGLContext *gl_context);
            void on_tick();
    };

}
