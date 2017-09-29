#include <set>
#include <iostream>

#include "../Object/Property/Drawable.hh"
#include "../Geometry/Vertex.hh"
#include "../AppContext.hh"
#include "../Utilities.hh"
#include "Quad.hh"
#include "Context.hh"
#include "Buffer.hh"
#include "Pipeline.hh"

using namespace Animate::VK;

Context::Context(std::weak_ptr<Animate::AppContext> context) : context(context)
{
    this->create_instance();
    this->bind_debug_callback();
    this->create_surface();
    this->pick_physical_device();
    this->create_logical_device();
    this->create_swap_chain();
    this->create_image_views();
    this->create_depth_stencil();
    this->create_render_pass();
    this->create_multisample_target();
    this->create_framebuffers();
    this->create_pipeline_layout();
    this->create_command_pool();
    this->create_descriptor_pool();
    this->create_command_buffers();
    this->create_semaphores();
    this->create_fences();
}

Context::~Context()
{
    if (this->descriptor_set_layout) {
        this->logical_device.destroyDescriptorSetLayout(this->descriptor_set_layout);
    }

    if (this->descriptor_pool) {
        this->logical_device.destroyDescriptorPool(this->descriptor_pool);
    }

    if (this->pipeline_layout) {
        this->logical_device.destroyPipelineLayout(this->pipeline_layout);
    }
    this->pipelines.clear();

    cleanup_swap_chain_dependancies();

    this->buffers.clear();

    if (this->swap_chain) {
        this->logical_device.destroySwapchainKHR(this->swap_chain);
    }

    if (this->image_available_semaphore) {
        this->logical_device.destroySemaphore(this->image_available_semaphore);
    }

    if (this->render_finished_semaphore) {
        this->logical_device.destroySemaphore(this->render_finished_semaphore);
    }

    if (this->command_pool) {
        this->logical_device.destroyCommandPool(this->command_pool);
    }

    for (size_t i = 0; i < this->render_fences.size(); i++) {
        if (this->render_fences[i]) {
            this->logical_device.destroyFence(this->render_fences[i], nullptr);
        }
    }

    if (this->logical_device) {
        this->logical_device.destroy();
    }

    if (this->debug_callback_obj) {
        auto DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)this->instance.getProcAddr("vkDestroyDebugReportCallbackEXT");
        if (DestroyDebugReportCallback != nullptr) {
            DestroyDebugReportCallback(this->instance, this->debug_callback_obj, nullptr);
        } else {
            std::cerr << "Unable to desroy debug callback." << std::endl;
        }
    }

    if (this->instance) {
        this->instance.destroy();
    }
}

void Context::recreate_swap_chain()
{
    std::lock_guard<std::mutex> command_guard(this->command_mutex);

    this->logical_device.waitIdle();

    this->cleanup_swap_chain_dependancies();

    vk::SwapchainKHR tmp_swap_chain = this->swap_chain;

    this->create_swap_chain();

    this->logical_device.destroySwapchainKHR(tmp_swap_chain, nullptr);

    this->create_image_views();
    this->create_depth_stencil();
    this->create_render_pass();

    this->create_multisample_target();
    this->recreate_pipelines();

    this->create_framebuffers();
    this->create_command_buffers();
}

void Context::cleanup_swap_chain_dependancies()
{
    if (this->depth_image) {
        this->logical_device.destroyImage(this->depth_image);
    }

    if (this->depth_view) {
        this->logical_device.destroyImageView(this->depth_view);
    }

    if (this->depth_memory) {
        this->logical_device.freeMemory(this->depth_memory);
    }

    if (this->multisample_target.colour.image) {
        this->logical_device.destroyImage(this->multisample_target.colour.image);
    }

    if (this->multisample_target.colour.view) {
        this->logical_device.destroyImageView(this->multisample_target.colour.view);
    }

    if (this->multisample_target.colour.memory) {
        this->logical_device.freeMemory(this->multisample_target.colour.memory);
    }

    if (this->multisample_target.depth.image) {
        this->logical_device.destroyImage(this->multisample_target.depth.image);
    }

    if (this->multisample_target.depth.view) {
        this->logical_device.destroyImageView(this->multisample_target.depth.view);
    }

    if (this->multisample_target.depth.memory) {
        this->logical_device.freeMemory(this->multisample_target.depth.memory);
    }

    for (size_t i = 0; i < this->swap_chain_framebuffers.size(); i++) {
        if (this->swap_chain_framebuffers[i]) {
            this->logical_device.destroyFramebuffer(this->swap_chain_framebuffers[i], nullptr);
        }
    }

    if (this->command_pool) {
        this->logical_device.freeCommandBuffers(
            this->command_pool,
            static_cast<uint32_t>(this->command_buffers.size()),
            this->command_buffers.data()
        );
    }

    if (this->render_pass) {
        this->logical_device.destroyRenderPass(this->render_pass, nullptr);
    }

    for (size_t i = 0; i < this->swap_chain_image_views.size(); i++) {
        this->logical_device.destroyImageView(this->swap_chain_image_views[i], nullptr);
    }
}

void Context::recreate_pipelines()
{
    for(auto const& pipeline: this->pipelines) {
        pipeline->recreate_pipeline();
    }
}

void Context::fill_command_buffer(int i)
{
    vk::Rect2D render_area = vk::Rect2D(
        {0,0},
        this->swap_chain_extent
    );

    vk::ClearValue clear_values[3];
    clear_values[0] = vk::ClearValue()
        .setColor(vk::ClearColorValue().setFloat32({0.0f, 0.0f, 0.0f, 1.0f}));
    clear_values[1] = vk::ClearValue()
        .setColor(vk::ClearColorValue().setFloat32({0.0f, 0.0f, 0.0f, 1.0f}));
    clear_values[2] = vk::ClearValue()
        .setDepthStencil(vk::ClearDepthStencilValue(1.0f, 0));

    vk::Viewport viewport = vk::Viewport()
        .setWidth(this->swap_chain_extent.width)
        .setHeight(this->swap_chain_extent.height)
        .setMaxDepth(1.0f);

    vk::CommandBufferBeginInfo begin_info = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

    vk::RenderPassBeginInfo render_pass_begin_info = vk::RenderPassBeginInfo()
        .setRenderPass(this->render_pass)
        .setRenderArea(render_area)
        .setClearValueCount(3)
        .setPClearValues(clear_values)
        .setFramebuffer(this->swap_chain_framebuffers[i]);

    this->command_buffers[i].reset(vk::CommandBufferResetFlags());
    this->command_buffers[i].begin(&begin_info);
    this->command_buffers[i].setViewport(0, 1, &viewport);
    this->command_buffers[i].beginRenderPass(&render_pass_begin_info, vk::SubpassContents::eInline);

    vk::DeviceSize index_count = 0;
    vk::DeviceSize offsets[] = {0};
    vk::Buffer  last_vertex_buffer,
                last_index_buffer;
    bool first_pipeline_draw = true;

    for(auto const& pipeline: this->pipelines) {
        first_pipeline_draw = true;

        std::vector< std::shared_ptr<Drawable> > drawables = pipeline->get_scene();

        for (auto const& drawable : drawables) {
            if (!drawable) {
                continue;
            }

            vk::Buffer vertex_buffer = drawable->get_vertex_buffer();
            vk::Buffer index_buffer = drawable->get_index_buffer();
            index_count = drawable->get_index_count();

            if (vertex_buffer && index_buffer && index_count > 0) {
                Matrix model_matrix = drawable->get_model_matrix();

                if (first_pipeline_draw) {
                    this->command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline.get());

                    vk::DescriptorSet descriptor_set = pipeline->get_descriptor_set();

                    this->command_buffers[i].bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        this->pipeline_layout,
                        0,
                        1,
                        &descriptor_set,
                        0,
                        nullptr
                    );

                    first_pipeline_draw = false;
                }

                if (last_vertex_buffer != vertex_buffer) {
                    this->command_buffers[i].bindVertexBuffers(0, 1, &vertex_buffer, offsets);
                    last_vertex_buffer = vertex_buffer;
                }

                if (last_index_buffer != index_buffer) {
                    this->command_buffers[i].bindIndexBuffer(index_buffer, 0, vk::IndexType::eUint16);
                    last_index_buffer = index_buffer;
                }

                Matrix mvp = pipeline->get_matrix() * drawable->get_model_matrix();

                //Set push constants
                this->command_buffers[i].pushConstants(
                    this->pipeline_layout,
                    vk::ShaderStageFlagBits::eVertex,
                    0,
                    sizeof(float)*16,
                    &mvp
                );

                this->command_buffers[i].drawIndexed(index_count, 1, 0, 0, 0);
            }
        }
    }

    this->command_buffers[i].endRenderPass();
    this->command_buffers[i].end();
}

void Context::commit_scenes()
{
    for(auto const& pipeline : this->pipelines) {
        pipeline->commit_scene();
    }
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

    vk::Result fence_result = this->logical_device.waitForFences(
        1,
        &this->render_fences[image_index],
        VK_TRUE,
        1000000000
    );

    switch (fence_result) {
        case vk::Result::eTimeout:
            std::cout << "Fence timeout." << std::endl;
            this->logical_device.waitIdle();
            break;
        case vk::Result::eSuccess:
            break;
        default:
            throw std::runtime_error("Error waiting for fence: " + vk::to_string(fence_result));
    }

    this->logical_device.resetFences(1, &this->render_fences[image_index]);

    vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::SubmitInfo submit_info = vk::SubmitInfo()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&this->image_available_semaphore)
        .setPWaitDstStageMask(wait_stages)
        .setCommandBufferCount(1)
        .setPCommandBuffers(&this->command_buffers[image_index])
        .setSignalSemaphoreCount(1)
        .setPSignalSemaphores(&this->render_finished_semaphore);

    vk::PresentInfoKHR present_info = vk::PresentInfoKHR()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&this->render_finished_semaphore)
        .setSwapchainCount(1)
        .setPSwapchains(&this->swap_chain)
        .setPImageIndices(&image_index);

    std::lock_guard<std::mutex> resource_guard(this->vulkan_resource_mutex);
    std::lock_guard<std::mutex> command_guard(this->command_mutex);
    this->fill_command_buffer(image_index);
    if (this->graphics_queue.submit(1, &submit_info, this->render_fences[image_index]) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't submit to graphics queue.");
    }

    if (this->present_queue.presentKHR(&present_info) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't submit to present queue.");
    }
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

    vk::Result result = vk::createInstance(&create_info, nullptr, &this->instance);
    if (result != vk::Result::eSuccess) {
       throw std::runtime_error("Couldn't create vulkan instance: " + vk::to_string(result));
    }
}

void Context::bind_debug_callback()
{
    //Set the debug callback
    vk::DebugReportCallbackCreateInfoEXT debug_create_info = vk::DebugReportCallbackCreateInfoEXT()
    .setFlags(
        //vk::DebugReportFlagBitsEXT::eInformation |
        vk::DebugReportFlagBitsEXT::eWarning |
        vk::DebugReportFlagBitsEXT::ePerformanceWarning |
        vk::DebugReportFlagBitsEXT::eError |
        vk::DebugReportFlagBitsEXT::eDebug
    )
    .setPfnCallback((PFN_vkDebugReportCallbackEXT)debug_callback);

    auto CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)this->instance.getProcAddr("vkCreateDebugReportCallbackEXT");
    if (CreateDebugReportCallback != nullptr) {
        const VkDebugReportCallbackCreateInfoEXT tmp(debug_create_info);
        CreateDebugReportCallback(this->instance, &tmp, nullptr, &this->debug_callback_obj);
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

    vk::PhysicalDeviceProperties properties;

    //Just pick the first device for now
    for (auto const& device : devices) {
        properties = device.getProperties();
        if (this->is_device_suitable(device)) {
            this->physical_device = device;
            break;
        }
    }

    if (!this->physical_device) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    std::cout << "Using device: " << properties.deviceName << std::endl;

    this->multisample_target.sample_count = this->choose_sample_count(this->physical_device.getProperties());
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

    vk::PhysicalDeviceFeatures features = vk::PhysicalDeviceFeatures()
        .setSamplerAnisotropy(VK_TRUE)
        .setSampleRateShading(VK_TRUE)
        .setAlphaToOne(VK_TRUE);

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
        .setSurface(*this->context.lock()->get_surface().lock().get())
        .setMinImageCount(image_count)
        .setImageFormat(surface_format.format)
        .setImageColorSpace(surface_format.colorSpace)
        .setImageExtent(extent)
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setPreTransform(swap_chain_support.capabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(present_mode)
        .setClipped(VK_TRUE)
        .setOldSwapchain(this->swap_chain);

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
            .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

        if (this->logical_device.createImageView(&create_info, nullptr, &this->swap_chain_image_views.at(i)) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create image view.");
        }
    }
}

void Context::create_depth_stencil()
{
    this->depth_format = this->choose_depth_format(this->physical_device);

    vk::ImageCreateInfo image_create_info = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setFormat(this->depth_format)
        .setExtent(
            vk::Extent3D()
                .setWidth(this->swap_chain_extent.width)
                .setHeight(this->swap_chain_extent.height)
                .setDepth(1)
        )
        .setMipLevels(1)
        .setArrayLayers(1)
        .setTiling(vk::ImageTiling::eOptimal)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc);

    if (this->logical_device.createImage(&image_create_info, nullptr, &this->depth_image) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create depth stencil image.");
    }

    vk::MemoryRequirements memory_requirements;
    this->logical_device.getImageMemoryRequirements(this->depth_image, &memory_requirements);

    vk::MemoryAllocateInfo allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            this->find_memory_type(
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            )
        );

    if (this->logical_device.allocateMemory(&allocation_info, nullptr, &this->depth_memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate depth stencil image memory.");
    }
    this->logical_device.bindImageMemory(this->depth_image, this->depth_memory, 0);

    vk::ImageViewCreateInfo image_view_create_info = vk::ImageViewCreateInfo()
        .setImage(this->depth_image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(this->depth_format)
        .setComponents(
            vk::ComponentMapping(
                vk::ComponentSwizzle::eR,
                vk::ComponentSwizzle::eG,
                vk::ComponentSwizzle::eB,
                vk::ComponentSwizzle::eA
            )
        )
        .setSubresourceRange(
            vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil,
                0,
                1,
                0,
                1
            )
        );


    if (this->logical_device.createImageView(&image_view_create_info, nullptr, &this->depth_view) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create depth stencil image view.");
    }
}

void Context::create_render_pass()
{
    std::array<vk::AttachmentDescription, 4> attachments = {};

    attachments[0] = vk::AttachmentDescription()
        .setFormat(this->swap_chain_image_format)
        .setSamples(this->multisample_target.sample_count)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

    attachments[1] = vk::AttachmentDescription()
        .setFormat(this->swap_chain_image_format)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    attachments[2] = vk::AttachmentDescription()
        .setFormat(this->depth_format)
        .setSamples(this->multisample_target.sample_count)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    attachments[3] = vk::AttachmentDescription()
        .setFormat(this->depth_format)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentReference colour_attachment_reference = vk::AttachmentReference()
        .setAttachment(0)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::AttachmentReference depth_attachment_reference = vk::AttachmentReference()
        .setAttachment(2)
        .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentReference resolve_attachment_reference = vk::AttachmentReference()
        .setAttachment(1)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass = vk::SubpassDescription()
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachmentCount(1)
        .setPColorAttachments(&colour_attachment_reference)
        .setPResolveAttachments(&resolve_attachment_reference)
        .setPDepthStencilAttachment(&depth_attachment_reference);

    std::array<vk::SubpassDependency, 2> dependencies;

    dependencies[0] = vk::SubpassDependency()
        .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setDstSubpass(0)
        .setSrcStageMask(vk::PipelineStageFlagBits::eBottomOfPipe)
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite)
        .setDependencyFlags(vk::DependencyFlagBits::eByRegion);

    dependencies[1] = vk::SubpassDependency()
        .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setDstSubpass(0)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstStageMask(vk::PipelineStageFlagBits::eBottomOfPipe)
        .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite)
        .setDependencyFlags(vk::DependencyFlagBits::eByRegion);

    vk::RenderPassCreateInfo render_pass_create_info = vk::RenderPassCreateInfo()
        .setAttachmentCount(attachments.size())
        .setPAttachments(attachments.data())
        .setSubpassCount(1)
        .setPSubpasses(&subpass)
        .setDependencyCount(2)
        .setPDependencies(dependencies.data());

    if (this->logical_device.createRenderPass(
            &render_pass_create_info,
            nullptr,
            &this->render_pass
        ) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create render pass.");
    }
}

std::weak_ptr<Pipeline> Context::create_pipeline(
    std::string fragment_code_id,
    std::string vertex_code_id,
    std::vector<std::string> resources
) {
    std::shared_ptr<Pipeline> pipeline(
        new Pipeline(
            this->shared_from_this(),
            fragment_code_id,
            vertex_code_id,
            resources
        )
    );

    this->pipelines.push_back(pipeline);
    return pipeline;
}

std::weak_ptr<Buffer> Context::create_buffer(
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags properties
) {
    std::shared_ptr<Buffer> buffer(
        new Buffer(
            this->shared_from_this(),
            size,
            usage,
            properties
        )
    );

    this->buffers.insert(
        std::make_pair(
            buffer->get_id(),
            buffer
        )
    );
    return buffer;
}

std::weak_ptr<Buffer> Context::get_buffer(uint64_t id)
{
    std::map< uint64_t, std::shared_ptr<Buffer> >::const_iterator it;
    it = this->buffers.find(id);
    if (it != this->buffers.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Tried to get a non existant buffer.");
    }
}

void Context::release_buffer(std::weak_ptr<Buffer> buffer)
{
    std::lock_guard<std::mutex> guard(this->vulkan_resource_mutex);

    uint64_t id = buffer.lock()->get_id();
    std::map< uint64_t, std::shared_ptr<Buffer> >::const_iterator it;
    it = this->buffers.find(id);
    if (it != this->buffers.end()) {
        this->buffers.erase(it);
    }
}

void Context::run_one_time_commands(std::function<void(vk::CommandBuffer)> func)
{
    std::lock_guard<std::mutex> guard(this->command_mutex);

    vk::CommandBufferAllocateInfo allocation_info = vk::CommandBufferAllocateInfo()
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandPool(this->command_pool)
        .setCommandBufferCount(1);

    vk::CommandBuffer command_buffer;
    this->logical_device.allocateCommandBuffers(&allocation_info, &command_buffer);

    vk::CommandBufferBeginInfo begin_info = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    command_buffer.begin(&begin_info);

    func(command_buffer);

    command_buffer.end();

    vk::SubmitInfo submit_info = vk::SubmitInfo()
        .setCommandBufferCount(1)
        .setPCommandBuffers(&command_buffer);

    if (this->graphics_queue.submit(1, &submit_info, nullptr) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't submit to graphics queue.");
    }

    this->logical_device.waitIdle();
    this->logical_device.freeCommandBuffers(this->command_pool, 1, &command_buffer);
}

void Context::create_multisample_target()
{
    vk::ImageCreateInfo image_create_info = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setFormat(this->swap_chain_image_format)
        .setExtent(
            vk::Extent3D()
                .setWidth(this->swap_chain_extent.width)
                .setHeight(this->swap_chain_extent.height)
                .setDepth(1)
        )
        .setMipLevels(1)
        .setArrayLayers(1)
        .setSharingMode(vk::SharingMode::eExclusive)
        .setTiling(vk::ImageTiling::eOptimal)
        .setSamples(this->multisample_target.sample_count)
        .setUsage(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment)
        .setInitialLayout(vk::ImageLayout::eUndefined);

    if (this->logical_device.createImage(&image_create_info, nullptr, &this->multisample_target.colour.image) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create colour attachment image.");
    }

    vk::MemoryRequirements memory_requirements;
    this->logical_device.getImageMemoryRequirements(this->multisample_target.colour.image, &memory_requirements);
    vk::MemoryAllocateInfo allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            this->find_memory_type(
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            )
        );

    if (this->logical_device.allocateMemory(&allocation_info, nullptr, &this->multisample_target.colour.memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate colour attachment image memory.");
    }
    this->logical_device.bindImageMemory(this->multisample_target.colour.image, this->multisample_target.colour.memory, 0);

    vk::ImageViewCreateInfo image_view_create_info = vk::ImageViewCreateInfo()
        .setImage(this->multisample_target.colour.image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(this->swap_chain_image_format)
        .setComponents(
            vk::ComponentMapping(
                vk::ComponentSwizzle::eR,
                vk::ComponentSwizzle::eG,
                vk::ComponentSwizzle::eB,
                vk::ComponentSwizzle::eA
            )
        )
        .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    if (this->logical_device.createImageView(&image_view_create_info, nullptr, &this->multisample_target.colour.view) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create colour attachment image view.");
    }

    image_create_info
        .setFormat(this->depth_format)
        .setUsage(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eDepthStencilAttachment);

    if (this->logical_device.createImage(&image_create_info, nullptr, &this->multisample_target.depth.image) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create depth attachment image.");
    }

    this->logical_device.getImageMemoryRequirements(this->multisample_target.depth.image, &memory_requirements);
    allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            this->find_memory_type(
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            )
        );

    if (this->logical_device.allocateMemory(&allocation_info, nullptr, &this->multisample_target.depth.memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate depth attachment image memory.");
    }
    this->logical_device.bindImageMemory(this->multisample_target.depth.image, this->multisample_target.depth.memory, 0);

    image_view_create_info
        .setFormat(this->depth_format)
        .setImage(this->multisample_target.depth.image)
        .setSubresourceRange(
            vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil,
                0,
                1,
                0,
                1
            )
        );

        if (this->logical_device.createImageView(&image_view_create_info, nullptr, &this->multisample_target.depth.view) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create depth attachment image view.");
        }
}

void Context::create_framebuffers()
{
    this->swap_chain_framebuffers.resize(this->swap_chain_image_views.size());

    std::array<vk::ImageView, 4> attachments;

    attachments[0] = this->multisample_target.colour.view;
    attachments[2] = this->multisample_target.depth.view;
    attachments[3] = this->depth_view;

    vk::FramebufferCreateInfo framebuffer_create_info = vk::FramebufferCreateInfo()
        .setRenderPass(this->render_pass)
        .setAttachmentCount(4)
        .setPAttachments(attachments.data())
        .setWidth(this->swap_chain_extent.width)
        .setHeight(this->swap_chain_extent.height)
        .setLayers(1);

    for (size_t i = 0; i < this->swap_chain_image_views.size(); i++) {
        attachments[1] = this->swap_chain_image_views[i];

        if (this->logical_device.createFramebuffer(&framebuffer_create_info, nullptr, &this->swap_chain_framebuffers[i]) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create framebuffer.");
        }
    }
}

void Context::create_command_pool()
{
    QueueFamilyIndices indices = this->get_device_queue_families(this->physical_device);

    vk::CommandPoolCreateInfo command_pool_create_info = vk::CommandPoolCreateInfo()
        .setQueueFamilyIndex(indices.graphics_family)
        .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

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

    for (size_t i = 0; i < this->command_buffers.size(); i++) {
        this->fill_command_buffer(i);
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

void Context::create_fences()
{
    this->render_fences.resize(this->swap_chain_framebuffers.size());

    vk::FenceCreateInfo create_info = vk::FenceCreateInfo()
        .setFlags(vk::FenceCreateFlagBits::eSignaled);


    for (size_t i = 0; i < this->swap_chain_image_views.size(); i++) {
        if (this->logical_device.createFence(&create_info, nullptr, &this->render_fences[i]) != vk::Result::eSuccess) {
            throw std::runtime_error("Couldn't create fence.");
        }
    }
}

void Context::create_pipeline_layout()
{
    vk::DescriptorSetLayoutBinding uniform_layout_binding = vk::DescriptorSetLayoutBinding()
        .setBinding(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

    vk::DescriptorSetLayoutBinding sampler_layout_binding = vk::DescriptorSetLayoutBinding()
        .setBinding(1)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setStageFlags(vk::ShaderStageFlagBits::eFragment);

    std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uniform_layout_binding, sampler_layout_binding};

    vk::DescriptorSetLayoutCreateInfo set_layout_create_info = vk::DescriptorSetLayoutCreateInfo()
        .setBindingCount(bindings.size())
        .setPBindings(bindings.data());

    if (this->logical_device.createDescriptorSetLayout(&set_layout_create_info, nullptr, &this->descriptor_set_layout) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create descriptor set layout.");
    }

    vk::PushConstantRange push_constant_range = vk::PushConstantRange()
        .setStageFlags(vk::ShaderStageFlagBits::eVertex)
        .setOffset(0)
        .setSize(sizeof(float)*16);

    vk::PipelineLayoutCreateInfo layout_info = vk::PipelineLayoutCreateInfo()
        .setSetLayoutCount(1)
        .setPSetLayouts(&this->descriptor_set_layout)
        .setPushConstantRangeCount(1)
        .setPPushConstantRanges(&push_constant_range);

    if (this->logical_device.createPipelineLayout(&layout_info, nullptr, &this->pipeline_layout) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create pipeline layout.");
    }
}

void Context::create_descriptor_pool()
{
    std::array<vk::DescriptorPoolSize, 2> pool_sizes = {
        vk::DescriptorPoolSize()
            .setType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(4),
        vk::DescriptorPoolSize()
            .setType(vk::DescriptorType::eCombinedImageSampler)
            .setDescriptorCount(4)
    };

    vk::DescriptorPoolCreateInfo pool_create_info = vk::DescriptorPoolCreateInfo()
        .setPoolSizeCount(pool_sizes.size())
        .setPPoolSizes(pool_sizes.data())
        .setMaxSets(4);

    if (this->logical_device.createDescriptorPool(&pool_create_info, nullptr, &this->descriptor_pool) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create descriptor pool.");
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

    vk::PhysicalDeviceFeatures features = device.getFeatures();

    return
        features.samplerAnisotropy &&
        features.geometryShader &&
        queue_families.is_complete() &&
        swap_chain_adequate;
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
        device.getSurfaceSupportKHR(i, *this->context.lock()->get_surface().lock().get(), &present_supported);

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

    vk::SurfaceKHR surface = *this->context.lock()->get_surface().lock().get();

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

vk::SampleCountFlagBits Context::choose_sample_count(VkPhysicalDeviceProperties properties)
{
    vk::SampleCountFlags counts = vk::SampleCountFlagBits(std::min(
        properties.limits.framebufferColorSampleCounts,
        properties.limits.framebufferDepthSampleCounts
    ));

    if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
    if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
    if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
    if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
    if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
    if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }
    return vk::SampleCountFlagBits::e1;
}

vk::Format Context::choose_depth_format(vk::PhysicalDevice physical_device)
{
    std::vector<vk::Format> depth_formats = {
        vk::Format::eD32SfloatS8Uint,
        vk::Format::eD32Sfloat,
        vk::Format::eD24UnormS8Uint,
        vk::Format::eD16UnormS8Uint,
        vk::Format::eD16Unorm
    };

    for (const auto& format : depth_formats) {
        vk::FormatProperties properties;
        physical_device.getFormatProperties(format, &properties);

        if (properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
            return format;
        }
    }

    throw std::runtime_error("No suitable depth stencil attachment format available.");
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
