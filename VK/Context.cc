#include <set>
#include <iostream>

#include "AppContext.hh"
#include "Context.hh"
#include "Utilities.hh"
#include "../Geometry/Vertex.hh"
#include "../Object/Property/Drawable.hh"

using namespace Animate::VK;

Context::Context(std::shared_ptr<Animate::AppContext> context)
{
    this->context = std::weak_ptr<Animate::AppContext>(context);

    this->create_instance();
    this->bind_debug_callback();
    this->create_surface();
    this->pick_physical_device();
    this->create_logical_device();
    this->create_swap_chain();
    this->create_image_views();
    this->create_render_pass();
    this->create_pipeline();
    this->create_framebuffers();
    this->create_command_pool();
    this->create_command_buffers();
    this->create_semaphores();

    this->is_complete = true;
}

void Context::recreate_swap_chain()
{
    this->logical_device.waitIdle();

    this->cleanup_swap_chain_dependancies();

    vk::SwapchainKHR tmp_swap_chain = this->swap_chain;

    this->create_swap_chain();

    this->logical_device.destroySwapchainKHR(tmp_swap_chain, nullptr);
    
    this->create_image_views();
    this->create_render_pass();
    this->create_pipeline();
    this->create_framebuffers();
    this->create_command_buffers();
}

void Context::cleanup_swap_chain_dependancies()
{
    for (size_t i = 0; i < this->swap_chain_framebuffers.size(); i++) {
        this->logical_device.destroyFramebuffer(this->swap_chain_framebuffers[i], nullptr);
    }

    this->logical_device.freeCommandBuffers(this->command_pool, static_cast<uint32_t>(this->command_buffers.size()), this->command_buffers.data());

    this->logical_device.destroyPipeline(this->pipeline, nullptr);
    this->logical_device.destroyPipelineLayout(this->pipeline_layout, nullptr);
    this->logical_device.destroyRenderPass(this->render_pass, nullptr);

    for (size_t i = 0; i < this->swap_chain_image_views.size(); i++) {
        this->logical_device.destroyImageView(this->swap_chain_image_views[i], nullptr);
    }
}

void Context::add_shader_stage(vk::ShaderStageFlagBits type, std::string resource_id)
{
    size_t code_size;

    const uint32_t *code = reinterpret_cast<const uint32_t*>(Utilities::get_resource_as_bytes(resource_id, &code_size));

    vk::ShaderModuleCreateInfo shader_module_create_info = vk::ShaderModuleCreateInfo()
        .setCodeSize(code_size)
        .setPCode(code);

    vk::ShaderModule shader_module;

    if (this->logical_device.createShaderModule(&shader_module_create_info, nullptr, &shader_module)!= vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create fragment shader module.");
    }

    vk::PipelineShaderStageCreateInfo shader_stage_info = vk::PipelineShaderStageCreateInfo()
        .setStage(type)
        .setModule(shader_module)
        .setPName("main");

    this->shader_stages.push_back(shader_stage_info);
}

void Context::add_to_scene(Drawable *drawable)
{
    this->scene.insert(drawable);
}

void Context::render_scene()
{
    uint32_t image_index;
    vk::Result result = this->logical_device.acquireNextImageKHR(
        this->swap_chain,
        std::numeric_limits<uint64_t>::max(),
        this->image_available_semaphore,
        nullptr,
        &image_index
    );

    switch (result) {
        //Success
        case vk::Result::eSuccess:
        case vk::Result::eSuboptimalKHR:
            break;

        //Invalid swapchain
        case vk::Result::eErrorOutOfDateKHR:
            this->recreate_swap_chain();
            break;

        //Other unhandled error
        default:
            throw std::runtime_error("Couldn't acquire swap chain image.");
    }

    vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::SubmitInfo submit_info = vk::SubmitInfo()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&this->image_available_semaphore)
        .setPWaitDstStageMask(wait_stages)
        .setCommandBufferCount(1)
        .setPCommandBuffers(&this->command_buffers[image_index])
        .setSignalSemaphoreCount(1)
        .setPSignalSemaphores(&this->render_finished_semaphore);

    if (this->graphics_queue.submit(1, &submit_info, nullptr) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't submit to graphics queue.");
    }

    vk::PresentInfoKHR present_info = vk::PresentInfoKHR()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&this->render_finished_semaphore)
        .setSwapchainCount(1)
        .setPSwapchains(&this->swap_chain)
        .setPImageIndices(&image_index);

    this->present_queue.waitIdle();

    if (this->present_queue.presentKHR(&present_info) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't submit to present queue.");
    }

    this->scene.clear();
}

uint32_t Context::find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memory_properties;
    this->physical_device.getMemoryProperties(&memory_properties);
    
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        if (
            type_filter & (1 << i) &&
            (memory_properties.memoryTypes[i].propertyFlags & properties) == properties
        ) {
            return i;
        }
    }

    throw std::runtime_error("Unable to find suitable memory type.");
}

void Context::create_instance()
{
    //Check that the required extensions are available
    if (!this->check_instance_extensions()) {
        throw std::runtime_error("Missing required vulkan extension.");
    }

    //Check for required layers
    if (!this->check_instance_layers()) {
        throw std::runtime_error("Missing required vulkan layer.");
    }

    //Get the required extensions and layers
    std::vector<const char*> extensions = this->get_required_instance_extensions();
    std::vector<const char*> layers = this->get_required_instance_layers();

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

    vk::createInstance(&create_info, nullptr, &this->instance);
}

void Context::bind_debug_callback()
{
    //Set the debug callback
    vk::DebugReportCallbackCreateInfoEXT debug_create_info = vk::DebugReportCallbackCreateInfoEXT()
    .setFlags(vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug)
    .setPfnCallback((PFN_vkDebugReportCallbackEXT)debug_callback);

    auto CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)this->instance.getProcAddr("vkCreateDebugReportCallbackEXT");
    if (CreateDebugReportCallback != nullptr) {
        VkDebugReportCallbackEXT callback;
        const VkDebugReportCallbackCreateInfoEXT tmp(debug_create_info);
        CreateDebugReportCallback(this->instance, &tmp, nullptr, &callback);
    } else {
        throw std::runtime_error("Cannot find required vkCreateDebugReportCallbackEXT function.");
    }
}

void Context::create_surface()
{
    VkSurfaceKHR surface;
    VkResult result = glfwCreateWindowSurface(this->instance, this->context.lock()->get_window(), NULL, &surface);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Couldn't create window surface.");
    }
    this->context.lock()->set_surface(new vk::SurfaceKHR(surface));
}

void Context::pick_physical_device()
{
    uint32_t device_count = 0;
    this->instance.enumeratePhysicalDevices(&device_count, nullptr);
    if (device_count == 0) {
        throw std::runtime_error("No suitable GPU device found.");
    }

    std::vector<vk::PhysicalDevice> devices(device_count);
    this->instance.enumeratePhysicalDevices(&device_count, devices.data());

    //Just pick the first device for now
    for (auto const& device : devices) {
        if (this->is_device_suitable(device)) {
            this->physical_device = device;
            break;
        }
    }

    if (!this->physical_device) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void Context::create_logical_device()
{
    QueueFamilyIndices indices = this->get_device_queue_families(this->physical_device);

    float const priority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
    std::set<int> queue_families = {
        indices.graphics_family,
        indices.present_family
    };

    for (const int& queue_family : queue_families) {
        queue_create_infos.push_back(
            vk::DeviceQueueCreateInfo()
                .setQueueFamilyIndex(queue_family)
                .setQueueCount(1)
                .setPQueuePriorities(&priority)
        );
    }

    vk::PhysicalDeviceFeatures features = vk::PhysicalDeviceFeatures();

    std::vector<const char*> layers = this->get_required_instance_layers();
    std::vector<const char*> extensions = this->get_required_device_extensions();

    vk::DeviceCreateInfo device_create_info = vk::DeviceCreateInfo()
        .setPQueueCreateInfos(queue_create_infos.data())
        .setQueueCreateInfoCount(queue_create_infos.size())
        .setPEnabledFeatures(&features)
        .setEnabledLayerCount(layers.size())
        .setPpEnabledLayerNames(layers.data())
        .setEnabledExtensionCount(extensions.size())
        .setPpEnabledExtensionNames(extensions.data());

    this->physical_device.createDevice(&device_create_info, nullptr, &this->logical_device);

    this->graphics_queue = this->logical_device.getQueue(indices.graphics_family, 0);
    this->present_queue = this->logical_device.getQueue(indices.present_family, 0);
}

void Context::create_swap_chain()
{
    SwapChainSupportDetails swap_chain_support = get_swap_chain_support(this->physical_device);

    vk::SurfaceFormatKHR surface_format = this->choose_swap_surface_format(swap_chain_support.formats);
    vk::PresentModeKHR present_mode = this->choose_swap_present_mode(swap_chain_support.present_modes);
    vk::Extent2D extent = this->choose_swap_extent(swap_chain_support.capabilities);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR create_info = vk::SwapchainCreateInfoKHR()
        .setSurface(*this->context.lock()->get_surface().get())
        .setMinImageCount(image_count)
        .setImageFormat(surface_format.format)
        .setImageColorSpace(surface_format.colorSpace)
        .setImageExtent(extent)
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setPreTransform(swap_chain_support.capabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(present_mode)
        .setClipped(VK_TRUE);

    if (is_complete) {
        create_info.setOldSwapchain(this->swap_chain);
    }

    QueueFamilyIndices indices = get_device_queue_families(this->physical_device);
    uint32_t indices_arr[] = {(uint32_t) indices.graphics_family, (uint32_t) indices.present_family};

    if (indices.graphics_family != indices.present_family) {
        create_info.setImageSharingMode(vk::SharingMode::eConcurrent)
        .setQueueFamilyIndexCount(2)
        .setPQueueFamilyIndices(indices_arr);
    } else {
        create_info.setImageSharingMode(vk::SharingMode::eExclusive);
    }

    if (this->logical_device.createSwapchainKHR(&create_info, nullptr, &this->swap_chain) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create swap chain.");
    }

    this->logical_device.getSwapchainImagesKHR(this->swap_chain, &image_count, nullptr);

    this->swap_chain_images.resize(image_count);
    this->logical_device.getSwapchainImagesKHR(this->swap_chain, &image_count, this->swap_chain_images.data());

    this->swap_chain_image_format = surface_format.format;
    this->swap_chain_extent = extent;
}

void Context::create_image_views()
{
    this->swap_chain_image_views.resize(this->swap_chain_images.size());

    for (uint32_t i = 0; i < this->swap_chain_images.size(); i++) {
        vk::ImageViewCreateInfo create_info = vk::ImageViewCreateInfo()
            .setImage(this->swap_chain_images.at(i))
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(this->swap_chain_image_format)
            .setComponents(vk::ComponentMapping())
            .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

        if (this->logical_device.createImageView(&create_info, nullptr, &this->swap_chain_image_views.at(i)) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create image view.");
        }
    }
}

void Context::create_render_pass()
{
    vk::AttachmentDescription colour_attachment = vk::AttachmentDescription()
        .setFormat(this->swap_chain_image_format)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
        .setLoadOp(vk::AttachmentLoadOp::eClear);

    vk::AttachmentReference colour_attachment_reference = vk::AttachmentReference()
        .setAttachment(0)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass = vk::SubpassDescription()
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachmentCount(1)
        .setPColorAttachments(&colour_attachment_reference);

    vk::SubpassDependency subpass_dependency = vk::SubpassDependency()
        .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPassCreateInfo render_pass_create_info = vk::RenderPassCreateInfo()
        .setAttachmentCount(1)
        .setPAttachments(&colour_attachment)
        .setSubpassCount(1)
        .setPSubpasses(&subpass)
        .setDependencyCount(1)
        .setPDependencies(&subpass_dependency);

    if (this->logical_device.createRenderPass(&render_pass_create_info, nullptr, &this->render_pass) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create render pass.");
    }
}

void Context::create_pipeline()
{
    auto binding_description = Geometry::Vertex::get_binding_description();
    auto attribute_descriptions = Geometry::Vertex::get_attribute_descriptions();

    vk::PipelineVertexInputStateCreateInfo vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
        .setVertexBindingDescriptionCount(1)
        .setVertexAttributeDescriptionCount(attribute_descriptions.size())
        .setPVertexBindingDescriptions(&binding_description)
        .setPVertexAttributeDescriptions(attribute_descriptions.data());

    vk::PipelineInputAssemblyStateCreateInfo input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
        .setTopology(vk::PrimitiveTopology::eTriangleList);

    vk::Viewport viewport = vk::Viewport()
        .setWidth((float)this->swap_chain_extent.width)
        .setHeight((float)this->swap_chain_extent.height)
        .setMaxDepth(1.0f);

    vk::Rect2D scissor = vk::Rect2D(
        {0,0},
        this->swap_chain_extent
    );

    vk::PipelineViewportStateCreateInfo viewport_info = vk::PipelineViewportStateCreateInfo()
        .setViewportCount(1)
        .setPViewports(&viewport)
        .setScissorCount(1)
        .setPScissors(&scissor);

    vk::PipelineRasterizationStateCreateInfo rasteriser_info = vk::PipelineRasterizationStateCreateInfo()
        .setLineWidth(1.0f)
        .setCullMode(vk::CullModeFlagBits::eNone)
        .setFrontFace(vk::FrontFace::eClockwise);

    vk::PipelineMultisampleStateCreateInfo multisampling_state_info = vk::PipelineMultisampleStateCreateInfo();

    vk::PipelineColorBlendAttachmentState colour_blend_attachment_info = vk::PipelineColorBlendAttachmentState()
        .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
        .setBlendEnable(true)
        .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
        .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
        .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
        .setDstAlphaBlendFactor(vk::BlendFactor::eZero);

    vk::PipelineColorBlendStateCreateInfo colour_blend_info = vk::PipelineColorBlendStateCreateInfo()
        .setAttachmentCount(1)
        .setPAttachments(&colour_blend_attachment_info);

    vk::DynamicState dynamic_states[] = { vk::DynamicState::eViewport };

    vk::PipelineDynamicStateCreateInfo dynamic_state_info = vk::PipelineDynamicStateCreateInfo()
        .setDynamicStateCount(1)
        .setPDynamicStates(dynamic_states);

    vk::PipelineLayoutCreateInfo layout_info = vk::PipelineLayoutCreateInfo();

    if (this->logical_device.createPipelineLayout(&layout_info, nullptr, &this->pipeline_layout) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create pipeline layout.");
    }

    this->add_shader_stage(vk::ShaderStageFlagBits::eFragment, "/Animate/data/Default/shader.frag.spv");
    this->add_shader_stage(vk::ShaderStageFlagBits::eVertex, "/Animate/data/Default/shader.vert.spv");

    vk::GraphicsPipelineCreateInfo pipeline_create_info = vk::GraphicsPipelineCreateInfo()
        .setStageCount(2)
        .setPStages(this->shader_stages.data())
        .setPVertexInputState(&vertex_input_info)
        .setPInputAssemblyState(&input_assembly_info)
        .setPViewportState(&viewport_info)
        .setPRasterizationState(&rasteriser_info)
        .setPMultisampleState(&multisampling_state_info)
        .setPColorBlendState(&colour_blend_info)
        .setPDynamicState(&dynamic_state_info)
        .setLayout(this->pipeline_layout)
        .setRenderPass(this->render_pass)
        .setSubpass(0);

    if (this->logical_device.createGraphicsPipelines(nullptr, 1, &pipeline_create_info, nullptr, &this->pipeline) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create graphics pipeline.");
    }
}

void Context::create_framebuffers()
{
    this->swap_chain_framebuffers.resize(this->swap_chain_image_views.size());

    for (size_t i = 0; i < this->swap_chain_image_views.size(); i++) {
        vk::FramebufferCreateInfo framebuffer_create_info = vk::FramebufferCreateInfo()
            .setRenderPass(this->render_pass)
            .setAttachmentCount(1)
            .setPAttachments(&this->swap_chain_image_views[i])
            .setWidth(this->swap_chain_extent.width)
            .setHeight(this->swap_chain_extent.height)
            .setLayers(1);

        if (this->logical_device.createFramebuffer(&framebuffer_create_info, nullptr, &this->swap_chain_framebuffers[i]) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create framebuffer.");
        }
    }
}

void Context::create_command_pool()
{
    QueueFamilyIndices indices = this->get_device_queue_families(this->physical_device);

    vk::CommandPoolCreateInfo command_pool_create_info = vk::CommandPoolCreateInfo()
        .setQueueFamilyIndex(indices.graphics_family);

    if (this->logical_device.createCommandPool(&command_pool_create_info, nullptr, &this->command_pool) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create command pool.");
    }
}

void Context::create_command_buffers()
{
    this->command_buffers.resize(this->swap_chain_framebuffers.size());

    vk::CommandBufferAllocateInfo command_buffer_allocate_info = vk::CommandBufferAllocateInfo()
        .setCommandPool(this->command_pool)
        .setCommandBufferCount((uint32_t) this->command_buffers.size())
        .setLevel(vk::CommandBufferLevel::ePrimary);

    if (this->logical_device.allocateCommandBuffers(&command_buffer_allocate_info, this->command_buffers.data()) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create command buffers.");
    }

    vk::Rect2D render_area = vk::Rect2D(
        {0,0},
        this->swap_chain_extent
    );

    vk::ClearValue clear_colour = vk::ClearValue()
        .setColor(vk::ClearColorValue().setFloat32({0.0f, 0.0f, 0.0f, 1.0f}));

    vk::Viewport viewport = vk::Viewport()
        .setWidth(this->swap_chain_extent.width)
        .setHeight(this->swap_chain_extent.height)
        .setMaxDepth(1.0f);

    vk::CommandBufferBeginInfo begin_info = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

    vk::RenderPassBeginInfo render_pass_begin_info = vk::RenderPassBeginInfo()
        .setRenderPass(this->render_pass)
        .setRenderArea(render_area)
        .setClearValueCount(1)
        .setPClearValues(&clear_colour);

    for (size_t i = 0; i < this->command_buffers.size(); i++) {
        render_pass_begin_info.setFramebuffer(this->swap_chain_framebuffers[i]);

        this->command_buffers[i].begin(&begin_info);
        this->command_buffers[i].setViewport(0, 1, &viewport);
        this->command_buffers[i].beginRenderPass(&render_pass_begin_info, vk::SubpassContents::eInline);
        this->command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, this->pipeline);
        this->command_buffers[i].draw(3, 1, 0, 0);
        this->command_buffers[i].endRenderPass();
        this->command_buffers[i].end();
    }
}

void Context::create_semaphores()
{
    vk::SemaphoreCreateInfo create_info = vk::SemaphoreCreateInfo();

    if (this->logical_device.createSemaphore(&create_info, nullptr, &this->image_available_semaphore) != vk::Result::eSuccess ||
        this->logical_device.createSemaphore(&create_info, nullptr, &this->render_finished_semaphore) != vk::Result::eSuccess
    ) {
        throw std::runtime_error("Couldn't create semaphores.");
    }
}

bool Context::is_device_suitable(vk::PhysicalDevice const & device)
{
    QueueFamilyIndices queue_families = this->get_device_queue_families(device);
    bool extensions_suported = this->check_device_extensions(device);

    bool swap_chain_adequate = false;
    if (extensions_suported) {
        SwapChainSupportDetails swap_chain_support = get_swap_chain_support(device);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    //vk::PhysicalDeviceProperties properties = device.getProperties();
    vk::PhysicalDeviceFeatures features = device.getFeatures();

    return features.geometryShader && queue_families.is_complete() && swap_chain_adequate;
}

QueueFamilyIndices Context::get_device_queue_families(vk::PhysicalDevice const & device)
{
    uint32_t property_count = 0;
    device.getQueueFamilyProperties(&property_count, nullptr);

    std::vector<vk::QueueFamilyProperties> properties(property_count);
    device.getQueueFamilyProperties(&property_count, properties.data());

    QueueFamilyIndices indices;
    int i = 0;
    for (const auto& property : properties) {
        //Check whether our surface can be drawn to with this device
        vk::Bool32 present_supported = false;
        device.getSurfaceSupportKHR(i, *this->context.lock()->get_surface().get(), &present_supported);

        if (property.queueCount > 0) {
            if (property.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphics_family = i;
            }

            if (present_supported) {
                indices.present_family = i;
            }
        }

        if (indices.is_complete()) {
            break;
        }

        i++;
    }
    return indices;
}

SwapChainSupportDetails Context::get_swap_chain_support(vk::PhysicalDevice const & device)
{
    SwapChainSupportDetails details;

    vk::SurfaceKHR surface = *this->context.lock()->get_surface().get();

    device.getSurfaceCapabilitiesKHR(surface, &details.capabilities);

    uint32_t format_count;
    device.getSurfaceFormatsKHR(surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        device.getSurfaceFormatsKHR(surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    device.getSurfacePresentModesKHR(surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        device.getSurfacePresentModesKHR(surface, &present_mode_count, details.present_modes.data());
    }

    return details;
}

std::vector<const char*> Context::get_required_instance_extensions() const
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

std::vector<vk::ExtensionProperties> Context::get_avalable_instance_extensions() const
{
    uint32_t extension_count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<vk::ExtensionProperties> extensions(extension_count);
    vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    return extensions;
}

std::vector<const char*> Context::get_required_instance_layers() const
{
    return {
        "VK_LAYER_LUNARG_standard_validation"
    };
}

std::vector<vk::LayerProperties> Context::get_available_instance_layers() const
{
    uint32_t layer_count;
    vk::enumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<vk::LayerProperties> layers(layer_count);
    vk::enumerateInstanceLayerProperties(&layer_count, layers.data());

    return layers;
}

std::vector<const char*> Context::get_required_device_extensions() const
{
    return {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

std::vector<vk::ExtensionProperties> Context::get_available_device_extensions(vk::PhysicalDevice const & device) const
{
    uint32_t extension_count;
    device.enumerateDeviceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<vk::ExtensionProperties> extensions(extension_count);
    device.enumerateDeviceExtensionProperties(nullptr, &extension_count, extensions.data());

    return extensions;
}

bool Context::check_instance_extensions()
{
    //Get extensions required by glfw
    std::vector<const char*> const required_extensions = this->get_required_instance_extensions();

    //Get available extensions
    std::vector<vk::ExtensionProperties> const available_extensions = this->get_avalable_instance_extensions();

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

bool Context::check_instance_layers()
{
    std::vector<const char*> const required_layers = this->get_required_instance_layers();
    std::vector<vk::LayerProperties> const available_layers = this->get_available_instance_layers();


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

bool Context::check_device_extensions(vk::PhysicalDevice const & device)
{
    std::vector<vk::ExtensionProperties> available_extensions = this->get_available_device_extensions(device);
    std::vector<const char *> required_extensions_cstr = this->get_required_device_extensions();

    std::set<std::string> required_extensions(required_extensions_cstr.begin(), required_extensions_cstr.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

vk::SurfaceFormatKHR Context::choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats) const
{
    if (available_formats.size() == 1 && available_formats.front().format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto& available_format : available_formats) {
        if (available_format.format == vk::Format::eB8G8R8A8Unorm && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return available_format;
        }
    }

    return available_formats.front();
}

vk::PresentModeKHR Context::choose_swap_present_mode(std::vector<vk::PresentModeKHR> const & available_present_modes) const
{
    vk::PresentModeKHR chosen_mode = vk::PresentModeKHR::eFifo;

    for (const auto& present_mode : available_present_modes) {
        if (present_mode == vk::PresentModeKHR::eMailbox) {
            return present_mode;
        } else if (present_mode == vk::PresentModeKHR::eImmediate) {
            chosen_mode = present_mode;
        }
    }

    return chosen_mode;
}

vk::Extent2D Context::choose_swap_extent(vk::SurfaceCapabilitiesKHR const & capabilities) const
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetWindowSize(this->context.lock()->get_window(), &width, &height);

        vk::Extent2D actual_extent = {(uint32_t)width, (uint32_t)height};

        actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
        actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

        return actual_extent;
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Context::debug_callback(
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
