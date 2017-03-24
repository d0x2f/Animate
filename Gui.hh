#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include "Animation/Animation.hh"
#include "Context.hh"

using namespace Animate::Animation;

namespace Animate
{
    class Gui
    {
        public:
            Gui();
            virtual ~Gui();

            void start_loop();

            void on_key(int key, int scancode, int action, int mods);

        private:
            GLFWwindow *window;
            vk::Instance vk_instance;
            std::shared_ptr<Context> context;

            std::shared_ptr<Animation::Animation> noise_animation;
            std::vector< std::shared_ptr<Animation::Animation> >::iterator current_animation;

            std::vector< std::shared_ptr<Animation::Animation> > animations;

            void init_glfw();
            void init_vulkan();
            void init_context();
            void init_animations();

            std::vector<const char*> get_required_vulkan_extensions() const;
            std::vector<vk::ExtensionProperties> get_avalable_vulkan_extensions() const;
            std::vector<const char*> get_required_vulkan_layers() const;
            std::vector<vk::LayerProperties> get_available_vulkan_layers() const;

            bool check_vulkan_extensions();
            bool check_vulkan_layers();

            static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
                VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT object_type,
                uint64_t object,
                size_t location,
                int32_t code,
                const char* layer_prefix,
                const char* msg,
                void* user_data
            );

            void set_animation(Animation::Animation *animation);
    };
}
