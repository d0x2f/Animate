#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>

#include "Gui.hh"
#include "Animation/Cat/Cat.hh"
#include "Animation/Modulo/Modulo.hh"
#include "Animation/Noise/Noise.hh"

using namespace Animate;
using namespace Animate::Animation;

/**
 * Sets the window size and title.
 * Starts the cat animation.
 */
Gui::Gui()
{
    this->init_glfw();
    this->init_vulkan();
    this->init_context();
    this->init_animations();
}

/**
 * Destructor
 */
Gui::~Gui()
{
    glfwTerminate();
}

void Gui::init_glfw()
{
    glfwSetErrorCallback([](int error, const char* description){
        std::cout << description << " (" << error << ")" << std::endl;
    });

    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialise GLFW.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    this->window = glfwCreateWindow(1024, 1024, "Animate", nullptr, nullptr);
    if (!this->window) {
        throw std::runtime_error("Couldn't create GLFW window.");
    }

    //Set this window as the current context
    glfwMakeContextCurrent(this->window);

    //Disable VSync
    //glfwSwapInterval(0);

    glfwSetWindowUserPointer(this->window, this);

    glfwSetKeyCallback(
        this->window,
        [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            static_cast<Gui*>(glfwGetWindowUserPointer(w))->on_key(key, scancode, action, mods);
        }
    );
}

void Gui::init_vulkan()
{
    //Check that the required extensions are available
    if (!this->check_vulkan_extensions()) {
        throw std::runtime_error("Missing required vulkan extension.");
    }

    //Check for required layers
    if (!this->check_vulkan_layers()) {
        throw std::runtime_error("Missing required vulkan layer.");
    }

    //Set the debug callback
    vk::DebugReportCallbackCreateInfoEXT debug_create_info()
    .setFlags(VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT)
    .setPfnCallback(vulkan_debug_callback);

    //Get the required extensions and layers
    std::vector<const char*> extensions = this->get_required_vulkan_extensions();
    std::vector<const char*> layers = this->get_required_vulkan_layers();

    //Create a vulkan instance
    vk::ApplicationInfo app_info = vk::ApplicationInfo()
        .setPApplicationName("Animate")
        .setApplicationVersion(1)
        .setPEngineName("No Engine")
        .setEngineVersion(1)
        .setApiVersion(VK_API_VERSION_1_0);

    vk::InstanceCreateInfo create_info = vk::InstanceCreateInfo()
        .setPApplicationInfo(&app_info)
        .setEnabledExtensionCount(extensions.size())
        .setPpEnabledExtensionNames(extensions.data())
        .setEnabledLayerCount(layers.size())
        .setPpEnabledLayerNames(layers.data());

    vk::createInstance(&create_info, nullptr, &this->vk_instance);
}

void Gui::init_context()
{
    //Create context object
    this->context = std::shared_ptr<Context>( new Context() );

    //Create a texture manager
    new Textures(this->context.get());
}

void Gui::init_animations()
{
    //Create animation and connect it up
    this->noise_animation = std::shared_ptr<Animation::Animation>(new Animation::Noise::Noise(this->context.get()));
    noise_animation->initialise();

    Animation::Animation *animation = new Animation::Cat::Cat(this->context.get());
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    animation = new Animation::Modulo::Modulo(this->context.get());
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    this->current_animation = this->animations.begin()+1;
    (*this->current_animation)->start();
}

std::vector<const char*> Gui::get_required_vulkan_extensions() const
{
    std::vector<const char*> extensions;

    unsigned int glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    for (unsigned int i=0; i<glfw_extension_count; i++) {
        extensions.push_back(glfw_extensions[i]);
    }

    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    return extensions;
}

std::vector<vk::ExtensionProperties> Gui::get_avalable_vulkan_extensions() const
{
    uint32_t extension_count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<vk::ExtensionProperties> extensions(extension_count);
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    return extensions;
}

std::vector<const char*> Gui::get_required_vulkan_layers() const
{
    return {
        "VK_LAYER_LUNARG_standard_validation"
    };
}

std::vector<vk::LayerProperties> Gui::get_available_vulkan_layers() const
{
    uint32_t layer_count;
    vk::enumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<vk::LayerProperties> layers(layer_count);
    vk::enumerateInstanceLayerProperties(&layer_count, layers.data());

    return layers;
}

bool Gui::check_vulkan_extensions()
{
    //Get extensions required by glfw
    std::vector<const char*> const required_extensions = this->get_required_vulkan_extensions();

    //Get available extensions
    std::vector<vk::ExtensionProperties> const available_extensions = this->get_avalable_vulkan_extensions();

    //Check we have all the required extensions
    bool found;
    for (const auto& required_extension : required_extensions) {
        found = false;
        for (const auto& available_extension : available_extensions) {
            if (strcmp(required_extension, available_extension.extensionName) == 0){
                found = true;
            }
        }
        if (!found){
            return false;
        }
    }

    return true;
}

bool Gui::check_vulkan_layers()
{
    std::vector<const char*> const required_layers = this->get_required_vulkan_layers();
    std::vector<vk::LayerProperties> const available_layers = this->get_available_vulkan_layers();


    for (const auto& layer_name : required_layers) {
        bool found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Gui::vulkan_debug_callback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT object_type,
    uint64_t object,
    size_t location,
    int32_t code,
    const char* layer_prefix,
    const char* msg,
    void* user_data
) {
    std::cerr << "validation layer: " << msg << std::endl;

    return VK_FALSE;
}

void Gui::on_key(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        (*this->current_animation)->stop();

        this->current_animation++;
        if (this->current_animation == this->animations.end()) {
            this->current_animation = this->animations.begin();
        }

        (*this->current_animation)->start();
    }
}

/**
 * Start the gui loop.
 */
void Gui::start_loop()
{
    //Loop until the window is closed
    while (!glfwWindowShouldClose(this->window))
    {
        //Render the current animation if it's loaded, otherwise noise.
        Animation::Animation *animation = (*this->current_animation).get();
        if (animation->check_loaded()) {
            animation->on_render();
        } else {
            this->noise_animation->on_render();
        }

        //Swap buffers
        //glfwSwapBuffers(this->window);

        //Poll events
        glfwPollEvents();
    }
}
