#include <iostream>

#include "Animation.hh"
#include "Utilities.hh"

using namespace Animate::Animation;
using namespace Animate::Object;

/**
 * Constructor
 */
Animation::Animation(Context *context) : context(context)
{
}

/**
 * Destructor.
 * Ensure that the tick thread quits and joins.
 */
Animation::~Animation()
{
    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->running_mutex);
        this->running = false;
    }

    //If the thread exists and is joinable then join
    if (this->tick_thread && this->tick_thread.get()->joinable()) {
        this->tick_thread.get()->join();
    }
}

/**
 * Start the animation tick thread.
 */
void Animation::run()
{
    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->running_mutex);
        this->running = true;
    }

    this->tick_thread = std::unique_ptr<std::thread>( new std::thread(Animation::tick_loop, this) );
}

/**
 * Return whether the tick thread is runnin or not.
 *
 * @return bool
 */
bool Animation::check_running()
{
    std::lock_guard<std::mutex> guard(this->running_mutex);
    return this->running;
}

/**
 * Return the tick rate for this animation.
 *
 * @return int
 */
int Animation::get_tick_rate()
{
    return this->tick_rate;
}

bool Animation::on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context)
{
    GLuint64 current_time = Utilities::get_micro_time();
    this->frame_count++;
    if (current_time - this->last_frame_time >= 1000000) {
        std::cout << "Frame time: " << 1000000./static_cast<GLfloat>(this->frame_count) << std::endl;
        this->frame_count = 0;
        this->last_frame_time = current_time;
    }
}

/**
 * Run a loop that calls the animations on_tick method.
 * Quit when check_running() returns false.
 */
void Animation::tick_loop(Animation *animation)
{
    GLuint64 last_tick_time = Utilities::get_micro_time();
    while (animation->check_running()) {
        //Supply a time difference since the last tick
        GLuint64 tick_time = Utilities::get_micro_time();
        animation->on_tick(tick_time - last_tick_time);
        last_tick_time = tick_time;

        usleep(1000000. / animation->get_tick_rate());
    }
}


/**
 * Fetch an object from the list.
 *
 * @param name      The name of the object.
 *
 * @return the object.
 */
Object *Animation::get_object(std::string name)
{
    std::map< std::string, std::unique_ptr<Object::Object> >::iterator it;
    it = this->objects.find(name);
    if (it == this->objects.end()) {
        g_assert_not_reached();
    }
    return it->second.get();
}

/**
 * Add an object to the list.
 *
 * @param name      The name of the object.
 * @param object    The object to add.
 */
void Animation::add_object(std::string name, Object::Object *object)
{
    this->objects.insert( std::pair<std::string, std::unique_ptr<Object::Object> >(name, object));
}
