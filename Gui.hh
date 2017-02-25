#pragma once

#include <gtkmm.h>

#include "Animation/Animation.hh"
#include "Context.hh"
#include "GL/Area.hh"

using namespace Animate::Animation;

namespace Animate
{
    class Gui : public Gtk::Window
    {
        public:
            Gui();
            virtual ~Gui();

        private:
            std::unique_ptr<Context> context;
            std::unique_ptr<Animation::Animation> current_animation;

            void set_animation(Animation::Animation *animation);
    };
}
