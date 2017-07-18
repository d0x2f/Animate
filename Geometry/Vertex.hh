#pragma once

#include <array>

#include "Definitions.hh"

namespace Animate::Geometry
{
    /**
     * A single vertex.
     */
    struct Vertex
    {
            Vector3 position;
            Vector2 texture;
            Vector3 normal;
            Vector4 colour;

            Vertex(Vector3 p, Vector2 t, Vector3 n, Vector4 c) : position(p), texture(t), normal(n), colour(c) {}

            static vk::VertexInputBindingDescription get_binding_description()
            {
                return vk::VertexInputBindingDescription()
                    .setBinding(0)
                    .setStride(sizeof(GLfloat) * 12)
                    .setInputRate(vk::VertexInputRate::eVertex);
            }

            static std::array<vk::VertexInputAttributeDescription, 4> get_attribute_descriptions()
            {
                //Set position data
                std::array<vk::VertexInputAttributeDescription, 4> attributes;
                attributes[0]
                    .setBinding(0)
                    .setLocation(0)
                    .setFormat(vk::Format::eR32G32B32Sfloat)
                    .setOffset(offsetof(Vertex, position));

                //Set texture data
                attributes[1]
                    .setBinding(0)
                    .setLocation(1)
                    .setFormat(vk::Format::eR32G32Sfloat)
                    .setOffset(offsetof(Vertex, texture));

                //Set normal data
                attributes[2]
                    .setBinding(0)
                    .setLocation(2)
                    .setFormat(vk::Format::eR32G32B32Sfloat)
                    .setOffset(offsetof(Vertex, normal));

                //Set colour data
                attributes[3]
                    .setBinding(0)
                    .setLocation(3)
                    .setFormat(vk::Format::eR32G32B32Sfloat)
                    .setOffset(offsetof(Vertex, colour));

                return attributes;
            }
    };
}
