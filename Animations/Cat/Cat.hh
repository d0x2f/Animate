#pragma once

#include "../Animation.hh"
#include "../GL/Area.hh"
#include "../GL/Quad.hh"
#include "../GL/Shader.hh"
#include "../GL/Definitions.hh"

namespace Animate
{
    class Cat : public Animation
    {
        public:
            Cat(Context *context);

            bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context);
            void on_tick(GLuint64 time_delta);
            void on_realise();

        protected:
            std::mutex tick_mutex;
            GL::Colour colour;
            std::unique_ptr<GL::Shader> shader;

        private:
            std::unique_ptr<GL::Quad> some_quad;
            GL::Vector3 some_quad_direction;
    };
}
