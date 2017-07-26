#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Object/Property/Drawable.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::VK
{
    class Context;

    class Pipeline
    {
        public:
            Pipeline(std::weak_ptr<Context> context, std::string fragment_code_id, std::string vertex_code_id);
            ~Pipeline();

            operator vk::Pipeline() const
            {
                return this->pipeline;
            }
            
            void recreate_pipeline();

            void set_matrices(Matrix view, Matrix projection);
            Matrix get_matrix();

            void add_drawable(std::weak_ptr<Drawable> drawable);
            void flush_scene();
            std::vector<std::weak_ptr<Drawable> > const& get_drawables();

        private:
            std::weak_ptr<Context> context;
            vk::Device logical_device;

            std::string fragment_code_id;
            std::string vertex_code_id;
            vk::Pipeline pipeline;
            Matrix pv;

            std::vector<std::weak_ptr<Drawable> > drawables;

            std::vector<vk::ShaderModule> shader_modules;
            std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

            void load_shader(vk::ShaderStageFlagBits type, std::string resource_id);
            void create_pipeline();
            void create_uniform_buffer();
    };
}
