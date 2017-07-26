#pragma once

#include "../../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate
{

    namespace VK
    {
        class Context;
        class Pipeline;
        class Texture;
        class Buffer;
    }

    namespace Object::Property
    {
        class Drawable : public std::enable_shared_from_this<Drawable>
        {
            public:
                Drawable(std::weak_ptr<VK::Context> context) : context(context) {};
                virtual ~Drawable() {};

                void initialise(std::weak_ptr<VK::Pipeline> pipeline, std::weak_ptr<VK::Texture> texture);
                void initialise(std::weak_ptr<VK::Pipeline> pipeline);
                void set_pipeline(std::weak_ptr<VK::Pipeline> pipeline);
                void set_texture(std::weak_ptr<VK::Texture> texture);

                virtual std::vector< std::weak_ptr<VK::Buffer> > const get_buffers();
                std::weak_ptr<VK::Pipeline> const get_pipeline();
                std::weak_ptr<VK::Texture> const get_texture();
                Matrix const get_model_matrix();

                virtual void set_model_matrix(Matrix model_matrix);
                virtual void add_to_scene();

                virtual void initialise_buffers() = 0;

            protected:
                std::weak_ptr<VK::Context> context;
                bool initialised = false;
                std::weak_ptr<VK::Pipeline> pipeline;
                std::weak_ptr<VK::Texture> texture;
                Matrix model_matrix;
        };
    }
}
