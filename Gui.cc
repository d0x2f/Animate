#include <memory>
#include <vector>
#include <iostream>

#include "Gui.hh"
#include "Animation/Cat/Cat.hh"
#include "Animation/Modulo/Modulo.hh"

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
    this->context = std::unique_ptr<Context>( new Context() );

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

    //Create animation and connect it up
    Animation::Modulo::Modulo *animation = new Animation::Modulo::Modulo(this->context.get());
    //Animation::Cat::Cat *animation = new Animation::Cat::Cat(this->context.get());
    animation->initialise();
    set_animation(animation);
}

/**
 * Destructor
 */
Gui::~Gui()
{
    glfwTerminate();
}

/**
 * Start the gui loop.
 */
void Gui::start_loop()
{
    //Loop until the window is closed
    while (!glfwWindowShouldClose(window))
    {
        //Render the current animation
        this->current_animation->on_render();

        //Swap buffers
        glfwSwapBuffers(window);

        //Poll events
        glfwPollEvents();
    }
}

/**
 * Set the currently running animation.
 * Unbinds existing render signals and connects the given animation.
 *
 * @param animation        The animation to play.
 */
void Gui::set_animation(Animation::Animation *animation)
{
    //Triggers an existing animation's destructor
    this->current_animation.reset(animation);
}
