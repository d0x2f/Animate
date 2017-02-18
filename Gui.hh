#pragma once

#include <gtkmm.h>

#include "Animation.hh"
#include "GL/Area.hh"

namespace Animate {

    class Gui : public Gtk::Window {
        public:
            Gui();
            virtual ~Gui();

        protected:
            std::unique_ptr<GL::Area> gl_area;
            std::unique_ptr<Animation> current_animation;

            void set_animation(Animation *animation);
    };

}
