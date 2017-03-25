#include <GLFW/glfw3.h>
#include <iostream>

#include "VulkanContext.hh"

using namespace Animate::VK;

void VulkanContext::initialise()
{
    this->create_instance();
    this->bind_debug_callback();
    this->pick_device();
}

void VulkanContext::create_instance()
{
    //Check that the required extensions are available
    if (!this->check_extensions()) {
        throw std::runtime_error("Missing required vulkan extension.");
    }

    //Check for required layers
    if (!this->check_layers()) {
        throw std::runtime_error("Missing required vulkan layer.");
    }

    //Get the required extensions and layers
    std::vector<const char*> extensions = this->get_required_extensions();
    std::vector<const char*> layers = this->get_required_layers();
    std::vector<const char*>::iterator it = extensions.begin();

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

void VulkanContext::bind_debug_callback()
{
    //Set the debug callback
    vk::DebugReportCallbackCreateInfoEXT debug_create_info = vk::DebugReportCallbackCreateInfoEXT()
    .setFlags(vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug)
    .setPfnCallback((PFN_vkDebugReportCallbackEXT)debug_callback);

    auto CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)this->vk_instance.getProcAddr("vkCreateDebugReportCallbackEXT");
    if (CreateDebugReportCallback != nullptr) {
        VkDebugReportCallbackEXT callback;
        const VkDebugReportCallbackCreateInfoEXT tmp(debug_create_info);
        CreateDebugReportCallback(this->vk_instance, &tmp, nullptr, &callback);
    } else {
        throw std::runtime_error("Cannot find required vkCreateDebugReportCallbackEXT function.");
    }
}

void VulkanContext::pick_device()
{
    uint32_t device_count = 0;
    this->vk_instance.enumeratePhysicalDevices(&device_count, nullptr);
    if (device_count == 0) {
        throw std::runtime_error("No suitable GPU device found.");
    }

    std::vector<vk::PhysicalDevice> devices(device_count);
    this->vk_instance.enumeratePhysicalDevices(&device_count, devices.data());

    //Just pick the first device for now
    for (auto const & device : devices) {
        if (this->is_device_suitable(device)) {
            this->selected_device = device;
            break;
        }
    }

    if (!this->selected_device) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool VulkanContext::is_device_suitable(vk::PhysicalDevice device)
{
    //vk::PhysicalDeviceProperties properties = device.getProperties();
    vk::PhysicalDeviceFeatures features = device.getFeatures();

    return features.geometryShader;
}

std::vector<const char*> VulkanContext::get_required_extensions() const
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

std::vector<vk::ExtensionProperties> VulkanContext::get_avalable_extensions() const
{
    uint32_t extension_count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<vk::ExtensionProperties> extensions(extension_count);
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    return extensions;
}

std::vector<const char*> VulkanContext::get_required_layers() const
{
    return {
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects"
    };
}

std::vector<vk::LayerProperties> VulkanContext::get_available_layers() const
{
    uint32_t layer_count;
    vk::enumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<vk::LayerProperties> layers(layer_count);
    vk::enumerateInstanceLayerProperties(&layer_count, layers.data());

    return layers;
}

bool VulkanContext::check_extensions()
{
    //Get extensions required by glfw
    std::vector<const char*> const required_extensions = this->get_required_extensions();

    //Get available extensions
    std::vector<vk::ExtensionProperties> const available_extensions = this->get_avalable_extensions();

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

bool VulkanContext::check_layers()
{
    std::vector<const char*> const required_layers = this->get_required_layers();
    std::vector<vk::LayerProperties> const available_layers = this->get_available_layers();


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

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::debug_callback(
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
