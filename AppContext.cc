#include "AppContext.hh"

using namespace Animate;

/**
* Set the window.
*
* @param window A GLFW window reference.
*/
void AppContext::set_window(GLFWwindow *window)
{
    this->window = window;
}

/**
* Set the surface.
*
* @param surface A surface reference.
*/
void AppContext::set_surface(vk::SurfaceKHR *surface)
{
    this->surface = std::shared_ptr<vk::SurfaceKHR>(surface);
}

/**
* Set the graphics context.
*
* @param graphics_context A graphics context.
*/
void AppContext::set_graphics_context(VK::Context graphics_context)
{
    this->graphics_context = graphics_context;
}

/**
 * Set the texture manager to track.
 *
 * @param textures A textures manager
 */
void AppContext::set_textures(VK::Textures *textures)
{
    this->textures = std::shared_ptr<VK::Textures>(textures);
}

/**
 * Retrieves the app window.
 */
GLFWwindow *AppContext::get_window()
{
    if (!this->window) {
        throw std::runtime_error("Attemped to get window before initialised.");
    }

    return this->window;
}

/**
 * Retrieves the tracked surface object.
 */
std::shared_ptr<vk::SurfaceKHR> const AppContext::get_surface()
{
    if (!this->surface) {
        throw std::runtime_error("Attemped to get surface object before initialised.");
    }

    return this->surface;
}

/**
 * Retrieves the tracked graphcis context object.
 */
VK::Context const AppContext::get_graphics_context()
{
    return this->graphics_context;
}

/**
 * Retrieves the tracked textures object.
 */
std::shared_ptr<VK::Textures> const AppContext::get_textures()
{
    if (!this->textures) {
        throw std::runtime_error("Attemped to get texture manager before initialised.");
    }

    return this->textures;
}
