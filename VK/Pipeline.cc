#include <iostream>

#include "Pipeline.hh"
#include "Context.hh"
#include "../Utilities.hh"
#include "../Geometry/Vertex.hh"
#include "Buffer.hh"

using namespace Animate::VK;

/**
 * Constructor.
 */
Pipeline::Pipeline(
    std::weak_ptr<VK::Context> context,
    std::string fragment_code_id,
    std::string vertex_code_id
) : context(context), fragment_code_id(fragment_code_id), vertex_code_id(vertex_code_id)
{
    this->logical_device = context.lock()->logical_device;
    this->load_shader(vk::ShaderStageFlagBits::eFragment, fragment_code_id);
    this->load_shader(vk::ShaderStageFlagBits::eVertex, vertex_code_id);
    this->create_pipeline();
    this->create_uniform_buffer();
    this->create_descriptor_set();
}

/**
 * Destructor.
 */
Pipeline::~Pipeline()
{
    this->logical_device.waitIdle();
    for(auto const& shader_module: this->shader_modules) {
        this->logical_device.destroyShaderModule(shader_module, nullptr);
    }

    this->logical_device.destroyPipeline(this->pipeline, nullptr);
}

void Pipeline::load_shader(vk::ShaderStageFlagBits type, std::string resource_id)
{
    size_t code_size;

    const uint32_t *code = reinterpret_cast<const uint32_t*>(Utilities::get_resource_as_bytes(resource_id, &code_size));

    vk::ShaderModuleCreateInfo shader_module_create_info = vk::ShaderModuleCreateInfo()
        .setCodeSize(code_size)
        .setPCode(code);

    vk::ShaderModule shader_module;

    if (this->context.lock()->logical_device.createShaderModule(&shader_module_create_info, nullptr, &shader_module)!= vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create shader module.");
    }

    vk::PipelineShaderStageCreateInfo shader_stage_info = vk::PipelineShaderStageCreateInfo()
        .setStage(type)
        .setModule(shader_module)
        .setPName("main");

    this->shader_modules.push_back(shader_module);
    this->shader_stages.push_back(shader_stage_info);

    std::cout << "Loaded shader: " << resource_id << std::endl;
}

void Pipeline::recreate_pipeline()
{
    vk::Pipeline tmp = this->pipeline;
    this->create_pipeline();
    if (tmp) {
        this->context.lock()->logical_device.destroyPipeline(tmp, nullptr);
    }
}

void Pipeline::create_pipeline()
{
    std::shared_ptr<Context> context = this->context.lock();

    auto binding_description = Geometry::Vertex::get_binding_description();
    auto attribute_descriptions = Geometry::Vertex::get_attribute_descriptions();

    vk::PipelineVertexInputStateCreateInfo vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
        .setVertexBindingDescriptionCount(1)
        .setVertexAttributeDescriptionCount(attribute_descriptions.size())
        .setPVertexBindingDescriptions(&binding_description)
        .setPVertexAttributeDescriptions(attribute_descriptions.data());

    vk::PipelineInputAssemblyStateCreateInfo input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
        .setTopology(vk::PrimitiveTopology::eTriangleStrip);

    vk::Viewport viewport = vk::Viewport()
        .setWidth((float)context->swap_chain_extent.width)
        .setHeight((float)context->swap_chain_extent.height)
        .setMaxDepth(1.0f);

    vk::Rect2D scissor = vk::Rect2D(
        {0,0},
        context->swap_chain_extent
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

    vk::GraphicsPipelineCreateInfo pipeline_create_info = vk::GraphicsPipelineCreateInfo()
        .setStageCount(this->shader_stages.size())
        .setPStages(this->shader_stages.data())
        .setPVertexInputState(&vertex_input_info)
        .setPInputAssemblyState(&input_assembly_info)
        .setPViewportState(&viewport_info)
        .setPRasterizationState(&rasteriser_info)
        .setPMultisampleState(&multisampling_state_info)
        .setPColorBlendState(&colour_blend_info)
        .setPDynamicState(&dynamic_state_info)
        .setLayout(context->pipeline_layout)
        .setRenderPass(context->render_pass)
        .setSubpass(0)
        .setBasePipelineHandle(this->pipeline)
        .setFlags(vk::PipelineCreateFlagBits::eAllowDerivatives);

    if (context->logical_device.createGraphicsPipelines(nullptr, 1, &pipeline_create_info, nullptr, &this->pipeline) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create graphics pipeline.");
    }
}

void Pipeline::create_descriptor_set()
{
    std::shared_ptr<Context> context = this->context.lock();

    vk::DescriptorSetAllocateInfo allocation_info = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(context->descriptor_pool)
        .setDescriptorSetCount(1)
        .setPSetLayouts(&context->descriptor_set_layout);

    vk::Result result = this->logical_device.allocateDescriptorSets(&allocation_info, &this->descriptor_set);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create descriptor set: " + vk::to_string(result));
    }

    vk::DescriptorBufferInfo buffer_info = vk::DescriptorBufferInfo()
        .setBuffer(this->uniform_buffer.lock()->get_ident())
        .setOffset(0)
        .setRange(this->uniform_buffer.lock()->get_size());

    vk::WriteDescriptorSet descriptor_write = vk::WriteDescriptorSet()
        .setDstSet(this->descriptor_set)
        .setDstBinding(0)
        .setDstArrayElement(0)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setDescriptorCount(1)
        .setPBufferInfo(&buffer_info);

    this->logical_device.updateDescriptorSets(1, &descriptor_write, 0, nullptr);

    context->run_one_time_commands([&](vk::CommandBuffer command_buffer){
        command_buffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            context->pipeline_layout,
            0,
            1,
            &descriptor_set,
            0,
            nullptr
        );
    });
}

vk::DescriptorSet Pipeline::get_descriptor_set()
{
    return this->descriptor_set;
}

void Pipeline::create_uniform_buffer()
{
    this->uniform_buffer = this->context.lock()->create_buffer(
        sizeof(float),
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );
}

void Pipeline::set_uniform_float(float value)
{
    void *data = this->uniform_buffer.lock()->map();
    memcpy(data, &value, sizeof(float));
    this->uniform_buffer.lock()->unmap();
}

/**
 * Save the given matrices.
 *
 * @param view The view matrix.
 * @param projection The projection matrix.
 */
void Pipeline::set_matrices(Matrix view, Matrix projection)
{
    std::lock_guard<std::mutex> guard(this->matrix_mutex);

    this->pv = projection * view;
}

Matrix Pipeline::get_matrix()
{
    std::lock_guard<std::mutex> guard(this->matrix_mutex);

    return this->pv;
}

uint64_t Pipeline::add_drawable(std::weak_ptr<Drawable> drawable)
{
    std::lock_guard<std::mutex> guard(this->drawable_mutex);

    this->staging_drawables.push_back(drawable);
}

std::vector< std::weak_ptr<Drawable> > Pipeline::get_drawables()
{
    std::lock_guard<std::mutex> guard(this->drawable_mutex);

    return this->staging_drawables;
}

std::vector< std::weak_ptr<Drawable> > Pipeline::get_scene()
{
    std::lock_guard<std::mutex> guard(this->drawable_mutex);
    return this->scene;
}

void Pipeline::commit_scene()
{
    std::lock_guard<std::mutex> guard(this->drawable_mutex);

    this->scene = this->staging_drawables;

    this->staging_drawables.clear();
}