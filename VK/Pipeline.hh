#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "Context.hh"
#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate::VK
{
    class Pipeline
    {
        public:
            Pipeline(std::weak_ptr<VK::Context> context, std::string fragment_code_id, std::string vertex_code_id);
            ~Pipeline();

            operator vk::Pipeline() const
            {
                return this->pipeline;
            }
            
            void recreate_pipeline();

            void set_matrices(Matrix view, Matrix projection);
            Matrix get_matrix();

        private:
            std::weak_ptr<VK::Context> context;
            vk::Device logical_device;

            std::string fragment_code_id;
            std::string vertex_code_id;
            vk::Pipeline pipeline;
            Matrix pv;

            std::vector<Drawable> drawables;

            std::vector<vk::ShaderModule> shader_modules;
            std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

            void load_shader(vk::ShaderStageFlagBits type, std::string resource_id);
            void create_pipeline();
            void create_uniform_buffer();
    };
}
