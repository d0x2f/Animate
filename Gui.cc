#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>

#include "Gui.hh"
#include "Animation/Cat/Cat.hh"
#include "Animation/Modulo/Modulo.hh"
#include "Animation/Noise/Noise.hh"

using namespace Animate;
using namespace Animate::Animation;

/**
 * Sets the window size and title.
 * Starts the cat animation.
 */
Gui::Gui()
{
    this->init_context();
    this->init_glfw();
    this->init_graphics();
    this->init_animations();
}

/**
 * Destructor
 */
Gui::~Gui()
{
    glfwTerminate();
}

void Gui::init_glfw()
{
    glfwSetErrorCallback([](int error, const char* description){
        std::cout << description << " (" << error << ")" << std::endl;
    });

    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialise GLFW.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);


    GLFWwindow *window = glfwCreateWindow(1024, 1024, "Animate", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Couldn't create GLFW window.");
    }
    this->context->set_window(window);

    //Disable VSync
    //glfwSwapInterval(0);

    glfwSetWindowUserPointer(this->context->get_window(), this);

    glfwSetKeyCallback(
        this->context->get_window(),
        [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            static_cast<Gui*>(glfwGetWindowUserPointer(w))->on_key(key, scancode, action, mods);
        }
    );
}

void Gui::init_graphics()
{
    VK::Initialiser initialiser(this->context);
    VK::Context context = initialiser.create_context();
    this->context->set_graphics_context(context);

    //Now that we have a context, provide it to the window.
    glfwMakeContextCurrent(this->context->get_window());
}

void Gui::init_context()
{
    //Create context object
    this->context = std::shared_ptr<AppContext>( new AppContext() );

    //Create a texture manager
    new Textures(this->context);
}

void Gui::init_animations()
{
    //Create animation and connect it up
    this->noise_animation = std::shared_ptr<Animation::Animation>(new Animation::Noise::Noise(this->context));
    noise_animation->initialise();

    Animation::Animation *animation = new Animation::Cat::Cat(this->context);
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    animation = new Animation::Modulo::Modulo(this->context);
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    this->current_animation = this->animations.begin()+1;
    (*this->current_animation)->start();
}

void Gui::on_key(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        (*this->current_animation)->stop();

        this->current_animation++;
        if (this->current_animation == this->animations.end()) {
            this->current_animation = this->animations.begin();
        }

        (*this->current_animation)->start();
    }
}

/**
 * Start the gui loop.
 */
void Gui::start_loop()
{
    //Loop until the window is closed
    while (!glfwWindowShouldClose(this->context->get_window()))
    {
        //Render the current animation if it's loaded, otherwise noise.
        Animation::Animation *animation = (*this->current_animation).get();
        if (animation->check_loaded()) {
            animation->on_render();
        } else {
            this->noise_animation->on_render();
        }

        //Swap buffers
        //glfwSwapBuffers(this->context->get_window());

        //Poll events
        glfwPollEvents();
    }
}
