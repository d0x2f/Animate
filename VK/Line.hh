#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Object/Property/Drawable.hh"
#include "../Object/Property/Movable.hh"
#include "../Object/Property/Scalable.hh"
#include "../Object/Property/Rotatable.hh"
#include "../Object/Property/Coloured.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::VK
{
    class Line : public Drawable, public Movable, public Scalable, public Rotatable, public Coloured
    {
        public:
            Line(Point position, Scale scale, Vector3 rotation, Colour colour, GLfloat thickness);
            ~Line();

            void draw(Matrix model_matrix, std::shared_ptr<VK::Context> context) override;

        protected:
            static GLuint vao_id, buffer_id, instance_count;
            GLfloat thickness;

            void initialise_buffers() override;
    };
}
