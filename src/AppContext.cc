#include "AppContext.hh"
#include "Animation/Animation.hh"
#include "Animation/Cat/Cat.hh"
#include "Animation/Modulo/Modulo.hh"
#include "Animation/Noise/Noise.hh"

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
    this->surface.reset(surface);
}

/**
* Set the graphics context.
*
* @param graphics_context A graphics context.
*/
void AppContext::set_graphics_context(std::shared_ptr<VK::Context> graphics_context)
{
    this->graphics_context.swap(graphics_context);
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

void AppContext::setup_animations() {
    std::shared_ptr<AppContext> self = this->shared_from_this();

    //Create animation and connect it up
    this->noise_animation.reset(new Animation::Noise::Noise(self));
    this->noise_animation->initialise();

    Animation::Animation *animation = new Animation::Cat::Cat(self);
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    animation = new Animation::Modulo::Modulo(self);
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    this->current_animation = this->animations.begin();
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
    (*this->current_animation)->unload();

    this->current_animation++;

    if (this->current_animation == this->animations.end()) {
        this->current_animation = this->animations.begin();
    }

    (*this->current_animation)->load();
}
