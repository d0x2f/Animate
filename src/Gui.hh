#pragma once

#include <gtkmm.h>

#include "Animation.hh"
#include "GLArea.hh"

namespace Animate {

    class Gui : public Gtk::Window {
        public:
            Gui();
            virtual ~Gui();

        protected:
            GLArea *gl_area;

            void set_animation(Animation *animation);
    };

}
