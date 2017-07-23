#include <iostream>

#include "Pipeline.hh"
#include "../Utilities.hh"
#include "../Geometry/Vertex.hh"

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
}

/**
 * Destructor.
 */
Pipeline::~Pipeline()
{
    for (
        std::vector<vk::ShaderModule>::iterator it = this->shader_modules.begin();
        it != this->shader_modules.end();
        it++
    ) {
        this->logical_device.destroyShaderModule(*it, nullptr);
    }

    this->logical_device.destroyPipeline(this->pipeline, nullptr);
    this->logical_device.destroyPipelineLayout(this->pipeline_layout, nullptr);
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
        throw std::runtime_error("Couldn't create fragment shader module.");
    }

    vk::PipelineShaderStageCreateInfo shader_stage_info = vk::PipelineShaderStageCreateInfo()
        .setStage(type)
        .setModule(shader_module)
        .setPName("main");

    this->shader_modules.push_back(shader_module);
    this->shader_stages.push_back(shader_stage_info);
}

void Pipeline::create_pipeline()
{

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
        .setWidth((float)this->context.lock()->swap_chain_extent.width)
        .setHeight((float)this->context.lock()->swap_chain_extent.height)
        .setMaxDepth(1.0f);

    vk::Rect2D scissor = vk::Rect2D(
        {0,0},
        this->context.lock()->swap_chain_extent
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

    if (this->context.lock()->logical_device.createPipelineLayout(&layout_info, nullptr, &this->pipeline_layout) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create pipeline layout.");
    }

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
        .setLayout(this->pipeline_layout)
        .setRenderPass(this->context.lock()->render_pass)
        .setSubpass(0);

    if (this->context.lock()->logical_device.createGraphicsPipelines(nullptr, 1, &pipeline_create_info, nullptr, &this->pipeline) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create graphics pipeline.");
    }
}

/**
 * Create buffer space for matrix uniforms.
 */
void Pipeline::create_uniform_buffer()
{
}

/**
 * Upload the given matrices to the uniform buffer.
 *
 * @param model The model matrix.
 * @param view The view matrix.
 * @param projection The projection matrix.
 */
void Pipeline::set_matrices(Matrix model, Matrix view, Matrix projection)
{
}

/**
 * Upload the given model matrix to the uniform buffer.
 *
 * @param mode The model matrix.
 */
void Pipeline::set_model_matrix(Matrix model)
{
}

void Pipeline::set_uniform(std::string name, GLfloat value)
{
    
}

void Pipeline::set_uniform(std::string name, Vector4 value)
{
    
}
