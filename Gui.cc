#include <memory>
#include <vector>
#include <iostream>

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
Gui::Gui() {
    glfwSetErrorCallback([](int error, const char* description){
        std::cout << description << " (" << error << ")" << std::endl;
    });

    if (!glfwInit()) {
        throw std::string("Couldn't initialise GLFW.");
    }

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(1024, 1024, "Animate", NULL, NULL);
    if (!this->window) {
        throw std::string("Couldn't create GLFW window.");
    }

    //Set this window as the current context
    glfwMakeContextCurrent(this->window);

    //Disable VSync
    glfwSwapInterval(0);

    //Initialise glew
    glewInit();

    //Create context object
    this->context = std::shared_ptr<Context>( new Context() );

    //Create a texture manager
    new Textures(this->context.get());

    //Print version information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    glfwSetWindowUserPointer(this->window, this);

    glfwSetKeyCallback(
        this->window,
        [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            static_cast<Gui*>(glfwGetWindowUserPointer(w))->on_key(key, scancode, action, mods);
        }
    );

    //Create animation and connect it up
    this->noise_animation = std::shared_ptr<Animation::Animation>(new Animation::Noise::Noise(this->context.get()));
    noise_animation->initialise();

    Animation::Animation *animation = new Animation::Cat::Cat(this->context.get());
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    animation = new Animation::Modulo::Modulo(this->context.get());
    animation->initialise();

    this->animations.push_back(std::shared_ptr<Animation::Animation>(animation));

    this->current_animation = this->animations.begin();
    (*this->current_animation)->start();
}

/**
 * Destructor
 */
Gui::~Gui()
{
    glfwTerminate();
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
    while (!glfwWindowShouldClose(this->window))
    {
        //Render the current animation if it's loaded, otherwise noise.
        Animation::Animation *animation = (*this->current_animation).get();
        if (animation->check_loaded()) {
            animation->on_render();
        } else {
            this->noise_animation->on_render();
        }

        //Swap buffers
        glfwSwapBuffers(this->window);

        //Poll events
        glfwPollEvents();
    }
}
