#pragma once

#include <map>
#include <taquinsolve.hh>

#include "../Animation.hh"
#include "../../GL/Area.hh"
#include "../../GL/Quad.hh"
#include "../../GL/Shader.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Object/Object.hh"
#include "Object/Tile.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;
using namespace Animate::Animation::Cat::Object;

namespace Animate::Animation::Cat
{
    class Cat : public Animation
    {
        public:
            Cat(Context *context);

            bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context);
            void on_tick(GLuint64 time_delta);
            void on_realise();

        protected:
            std::unique_ptr<GL::Shader> shader;
            std::queue<TaquinSolve::Moves> move_sequence;
            std::map<int, Tile *> tile_position_map;
            int zero_position = 0;
            int grid_size = 0;

            bool reset_puzzle_flag = false;
            void reset_puzzle();
    };
}
