#pragma once

#include "../Animation.hh"
#include "../../GL/Area.hh"
#include "../../GL/Quad.hh"
#include "../../GL/Shader.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Object/Object.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;

namespace Animate::Animation::Modulo
{
    class Modulo : public Animation
    {
        public:
            Modulo(Context *context);

            bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context);
            void on_tick(GLuint64 time_delta);
            void on_realise();

        protected:
            std::unique_ptr<GL::Shader> shader;
    };
}
