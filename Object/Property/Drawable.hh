#pragma once

#include "../../VK/Pipeline.hh"
#include "../../VK/Texture.hh"
#include "../../VK/Context.hh"
#include "../../VK/Buffer.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;
using namespace Animate::VK;

namespace Animate::Object::Property
{
    class Drawable : public std::enable_shared_from_this<Drawable>
    {
        public:
            Drawable(std::weak_ptr<VK::Context> context) : context(context) {};
            virtual ~Drawable() {};

            void initialise(std::weak_ptr<Pipeline> shader, std::weak_ptr<Texture> texture);
            void initialise(std::weak_ptr<Pipeline> shader);
            void set_shader(std::weak_ptr<Pipeline> shader);
            void set_texture(std::weak_ptr<Texture> texture);

            virtual std::vector< std::weak_ptr<Buffer> > const get_buffers();
            std::weak_ptr<Pipeline> const get_shader();
            std::weak_ptr<Texture> const get_texture();

            virtual void draw(Matrix model_matrix);

            virtual void initialise_buffers() = 0;

        protected:
            std::weak_ptr<VK::Context> context;
            bool initialised = false;
            std::weak_ptr<Pipeline> shader;
            std::weak_ptr<Texture> texture;
    };
}
