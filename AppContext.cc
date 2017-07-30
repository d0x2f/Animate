#include "AppContext.hh"
#include "Animation/Animation.hh"
#include "Animation/Cat/Cat.hh"
#include "Animation/Modulo/Modulo.hh"
#include "Animation/Noise/Noise.hh"

using namespace Animate;

AppContext::~AppContext()
{
}

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
void AppContext::set_graphics_context(VK::Context  *graphics_context)
{
    this->graphics_context = std::shared_ptr<VK::Context>(graphics_context);
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
std::weak_ptr<vk::SurfaceKHR> const AppContext::get_surface()
{
    if (!this->surface) {
        throw std::runtime_error("Attemped to get surface object before initialised.");
    }

    return this->surface;
}

/**
 * Retrieves the tracked graphcis context object.
 */
std::weak_ptr<VK::Context> const AppContext::get_graphics_context()
{
    if (!this->graphics_context) {
        throw std::runtime_error("Attemped to get graphics engine before initialised.");
    }

    return this->graphics_context;
}

/**
 * Retrieves the tracked textures object.
 */
std::weak_ptr<VK::Textures> const AppContext::get_textures()
{
    if (!this->textures) {
        throw std::runtime_error("Attemped to get texture manager before initialised.");
    }

    return this->textures;
}

void AppContext::setup_animations() {
    //Create animation and connect it up
    this->noise_animation = std::unique_ptr<Animation::Animation>(new Animation::Noise::Noise(this->shared_from_this()));
    this->noise_animation->initialise();

    Animation::Animation *animation = new Animation::Cat::Cat(this->shared_from_this());
    animation->initialise();

    this->animations.push_back(std::unique_ptr<Animation::Animation>(animation));

    animation = new Animation::Modulo::Modulo(this->shared_from_this());
    animation->initialise();

    this->animations.push_back(std::unique_ptr<Animation::Animation>(animation));

    this->current_animation = this->animations.begin()+1;
    this->next_animation();
}

size_t AppContext::get_animation_count()
{
    return this->animations.size();
}

std::weak_ptr<Animation::Animation> AppContext::get_current_animation()
{
    if ((*this->current_animation)->check_loaded()) {
        return *this->current_animation;
    } else {
        return this->noise_animation;
    }
}

void AppContext::next_animation()
{
    this->current_animation++;

    if (this->current_animation == this->animations.end()) {
        this->current_animation = this->animations.begin();
    }

    (*this->current_animation)->on_load();
}
