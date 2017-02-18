#pragma once

#include <gtkmm.h>

namespace Animate::GL {
    class Area : public Gtk::GLArea {
        public:
            Area();
            ~Area();
        
        protected:
            //Slots
            void on_realize();
    };
}
