#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>

#include "Gui.hh"
#include "Utilities.hh"
#include "VK/Context.hh"
#include "VK/Textures.hh"

using namespace Animate;
using namespace Animate::Animation;

std::mutex Gui::thread_sync_mutex;

/**
 * Sets the window size and title.
 * Starts the cat animation.
 */
Gui::Gui()
{
    this->init_context();
    this->init_glfw();
    this->init_graphics();
    this->context->setup_animations();
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


    GLFWwindow *window = glfwCreateWindow(1024, 1024, "Animate", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Couldn't create GLFW window.");
    }
    this->context->set_window(window);

    glfwSetWindowUserPointer(this->context->get_window(), this);

    glfwSetKeyCallback(
        this->context->get_window(),
        [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            static_cast<Gui*>(glfwGetWindowUserPointer(w))->on_key(key, scancode, action, mods);
        }
    );

    glfwSetWindowSizeCallback(
        this->context->get_window(),
        [](GLFWwindow* w, int width, int height) {
            static_cast<Gui*>(glfwGetWindowUserPointer(w))->on_window_resize(width, height);
        }
    );
}

void Gui::init_graphics()
{
    new VK::Context(this->context);
}

void Gui::init_context()
{
    //Create context object
    this->context = std::shared_ptr<AppContext>( new AppContext() );

    //Create a texture manager
    new VK::Textures(this->context);
}

void Gui::on_key(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        this->context->next_animation();
    }
}

void Gui::on_window_resize(int width, int height)
{
    this->context->get_graphics_context().lock()->recreate_swap_chain();
}

void Gui::start_loops()
{
    this->graphics_thread = std::thread(Gui::run_graphics_loop, this->context);
    this->run_tick_loop();

    context->should_close = true;

    if (this->graphics_thread.joinable()) {
        this->graphics_thread.join();
    }
}

/**
 * Start the gui loop.
 */
void Gui::run_graphics_loop(std::shared_ptr<AppContext> app_context)
{
    GLuint64 current_time, frame_count=0, last_frame_time=Utilities::get_micro_time();

    //Loop until the window is closed
    while (!app_context->should_close)
    {
        //Perform the render
        app_context->get_graphics_context().lock()->render_scene();

        current_time = Utilities::get_micro_time();
        frame_count++;
        if (current_time - last_frame_time >= 1000000) {
            std::cout << "Frame time: " << 1000000./static_cast<GLfloat>(frame_count) << " FPS: " << frame_count << std::endl;
            frame_count = 0;
            last_frame_time = current_time;
        }
    }

    //Get the graphics context again incase it's changed in the last.. millisecond?
    app_context->get_graphics_context().lock()->logical_device.waitIdle();
}

void Gui::run_tick_loop()
{
    int tick_rate = 60;
    GLuint64 last_tick_time = Utilities::get_micro_time();
    GLuint64 tick_time = Utilities::get_micro_time();
    GLuint64 tick_delta = 0;

    //Loop until the window is closed
    while (!glfwWindowShouldClose(this->context->get_window()))
    {
        tick_time = Utilities::get_micro_time();
        tick_delta = tick_time - last_tick_time;
        last_tick_time = tick_time;

        //Construct a frame for the current animation if it's loaded, otherwise noise.
        std::weak_ptr<Animation::Animation> current_animation = this->context->get_current_animation();
        current_animation.lock()->on_tick(tick_delta);

        this->context->get_graphics_context().lock()->commit_scenes();

        //Poll events
        glfwPollEvents();

        last_tick_time = Utilities::get_micro_time();

        if ((1000000. / tick_rate) > tick_delta) {
            usleep((1000000. / tick_rate) - tick_delta);
        }
    }
}