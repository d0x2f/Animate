#include "Context.hh"
#include "Utilities.hh"

using namespace Animate::VK;

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