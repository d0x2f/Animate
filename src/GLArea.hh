#pragma once

#include <gtkmm.h>

namespace Animate {
    class GLArea : public Gtk::GLArea {
        public:
            GLArea();
            ~GLArea();
        
        protected:
            //Slots
            void on_realize();
    };
}
