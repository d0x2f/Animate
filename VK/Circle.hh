#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Object/Property/Drawable.hh"
#include "../Object/Property/Movable.hh"
#include "../Object/Property/Scalable.hh"
#include "../Object/Property/Coloured.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::VK
{
    class Circle : public Drawable, public Movable, public Scalable, public Coloured
    {
        public:
            Circle(std::weak_ptr<VK::Context> context, Point position, Scale size, Colour colour, float thickness);
            ~Circle();

            void set_model_matrix(Matrix model_matrix) override;

        protected:
            uint32_t  vao_id = 0,
                    buffer_id = 0;
            float thickness;

            void initialise_buffers() override;
    };
}
