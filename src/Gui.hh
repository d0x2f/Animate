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
            std::unique_ptr<GLArea> gl_area;
            std::unique_ptr<Animation> current_animation;

            void set_animation(Animation *animation);
    };

}
