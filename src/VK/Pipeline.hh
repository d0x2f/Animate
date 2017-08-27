#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <mutex>

#include "Textures.hh"
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
            Pipeline(
                std::weak_ptr<Context> context,
                std::string fragment_code_id,
                std::string vertex_code_id,
                std::vector<std::string> resources
            );
            ~Pipeline();

            operator vk::Pipeline() const
            {
                return this->pipeline;
            }
            
            void recreate_pipeline();

            vk::DescriptorSet get_descriptor_set();

            void create_textures(std::vector<std::string> resources);
            std::weak_ptr<Textures> get_textures();

            void set_matrices(Matrix view, Matrix projection);
            Matrix get_matrix();

            void set_uniform_float(float value);

            uint64_t add_drawable(std::shared_ptr<Drawable> drawable);
            std::vector< std::shared_ptr<Drawable> > get_drawables();

            void commit_scene();
            std::vector< std::shared_ptr<Drawable> > get_scene();

        private:
            std::weak_ptr<Context> context;
            vk::Device logical_device;

            std::mutex drawable_mutex;
            std::mutex matrix_mutex;

            std::weak_ptr<Buffer> uniform_buffer;
            vk::DescriptorSet descriptor_set;
            std::shared_ptr<Textures> textures;

            std::string fragment_code_id;
            std::string vertex_code_id;
            vk::Pipeline pipeline;
            Matrix pv;

            std::vector<std::shared_ptr<Drawable> > staging_drawables;
            std::vector<std::shared_ptr<Drawable> > scene;

            std::vector<vk::ShaderModule> shader_modules;
            std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

            void load_shader(vk::ShaderStageFlagBits type, std::string resource_id);
            void create_pipeline();
            void create_descriptor_set();
            void create_uniform_buffer();
    };
}
