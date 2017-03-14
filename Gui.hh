#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Animation/Animation.hh"
#include "Context.hh"

using namespace Animate::Animation;

namespace Animate
{
    class Gui
    {
        public:
            Gui();
            virtual ~Gui();

            void start_loop();

        private:
            GLFWwindow *window;
            std::unique_ptr<Context> context;
            std::unique_ptr<Animation::Animation> current_animation;

            void set_animation(Animation::Animation *animation);
    };
}
