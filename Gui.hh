#pragma once

#include <gtkmm.h>

#include "Animation.hh"
#include "Context.hh"
#include "GL/Area.hh"

namespace Animate
{
    class Gui : public Gtk::Window
    {
        public:
            Gui();
            virtual ~Gui();

        private:
            std::unique_ptr<Context> context;
            std::unique_ptr<Animation> current_animation;

            void set_animation(Animation *animation);
    };
}
