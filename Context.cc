#include "Context.hh"

using namespace Animate;

/**
* Set the window.
*
* @param window A GLFW window reference.
*/
void Context::set_window(GLFWwindow *window)
{
    this->window = window;
}

/**
* Set the surface.
*
* @param surface A surface reference.
*/
void Context::set_surface(vk::SurfaceKHR *surface)
{
    this->surface = std::shared_ptr<vk::SurfaceKHR>(surface);
}

/**
* Set the vulkan manager reference.
*
* @param vulkan A vulkan manager reference.
*/
void Context::set_vulkan(VK::VulkanContext *vulkan)
{
    this->vulkan = std::shared_ptr<VK::VulkanContext>(vulkan);
}

/**
 * Set the texture manager to track.
 *
 * @param textures A textures manager
 */
void Context::set_textures(VK::Textures *textures)
{
    this->textures = std::shared_ptr<VK::Textures>(textures);
}

/**
 * Retrieves the app window.
 */
GLFWwindow *Context::get_window()
{
    if (!this->window) {
        throw std::runtime_error("Attemped to get window before initialised.");
    }

    return this->window;
}

/**
 * Retrieves the tracked surface object.
 */
std::shared_ptr<vk::SurfaceKHR> const Context::get_surface()
{
    if (!this->surface) {
        throw std::runtime_error("Attemped to get surface object before initialised.");
    }

    return this->surface;
}

/**
 * Retrieves the tracked vulkan object.
 */
std::shared_ptr<VK::VulkanContext> const Context::get_vulkan()
{
    if (!this->vulkan) {
        throw std::runtime_error("Attemped to get vulkan context before initialised.");
    }

    return this->vulkan;
}

/**
 * Retrieves the tracked textures object.
 */
std::shared_ptr<VK::Textures> const Context::get_textures()
{
    if (!this->textures) {
        throw std::runtime_error("Attemped to get texture manager before initialised.");
    }

    return this->textures;
}
