#pragma once

#include <vector>
#include <memory>

#include "../../../Object/Object.hh"
#include "../../../Object/Property/Coloured.hh"
#include "../../../VK/Line.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Object;

namespace Animate::Animation::Modulo::Object
{
    struct LineDescription {
        Vector3 from;
        GLfloat length;
        GLfloat z_rotation;

        LineDescription(Vector3 from, GLfloat length, GLfloat z_rotation) : from(from), length(length), z_rotation(z_rotation) {}
    };

    class Ring : public Object, public Animate::Object::Property::Coloured
    {
        public:
            Ring(std::weak_ptr<VK::Context> context, Point position, Scale size);

            void initialise(std::weak_ptr<Pipeline> shader);
            void draw(Matrix model_matrix) override;
            void on_tick(GLuint64 time_delta) override;

        private:
            GLfloat hue = 0;
            GLuint modulo = 1000;
            GLfloat factor = 1;
            std::vector<LineDescription> line_descriptions;

            std::unique_ptr<Line> line;
    };
}
