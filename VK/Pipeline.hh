#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <string>

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

            void load_shader(vk::ShaderStageFlagBits type, std::string resource_id);
            void create_pipeline();
            void create_uniform_buffer();

            void set_matrices(Matrix model, Matrix view, Matrix projection);
            void set_model_matrix(Matrix model);

            void set_uniform(std::string name, GLfloat value);
            void set_uniform(std::string name, Vector4 value);

        private:
            std::weak_ptr<VK::Context> context;
            std::string fragment_code_id;
            std::string vertex_code_id;
            vk::Pipeline pipeline;
            vk::PipelineLayout pipeline_layout;

            vk::Device logical_device;

            std::vector<vk::ShaderModule> shader_modules;
            std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
    };
}
