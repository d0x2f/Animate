#pragma once

#include <vector>
#include <memory>

#include "../../../Object/Object.hh"
#include "../../../Object/Property/Coloured.hh"
#include "../../../VK/Line.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Object;
using namespace Animate::VK;

namespace Animate::Animation::Modulo::Object
{
    struct LineDescription {
        Vector3 from;
        float length;
        float z_rotation;

        LineDescription(Vector3 from, float length, float z_rotation) : from(from), length(length), z_rotation(z_rotation) {}
    };

    class Ring : public Object, public Animate::Object::Property::Coloured
    {
        public:
            Ring(std::weak_ptr<Context> context, Point position, Scale size);

            void initialise(std::weak_ptr<Pipeline> shader);
            void on_tick(uint64_t time_delta) override;

        private:
            uint32_t modulo = 1000;
            float factor = 1;
            std::vector<std::shared_ptr<Line> > lines;
    };
}
