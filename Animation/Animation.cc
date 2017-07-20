#include <iostream>
#include <stdlib.h>

#include "Animation.hh"
#include "Utilities.hh"

using namespace Animate::Animation;
using namespace Animate::Object;

/**
 * Constructor
 */
Animation::Animation(std::weak_ptr<AppContext> context) : context(context)
{
    this->running = false;
    this->loaded = false;
}

/**
 * Destructor.
 * Ensure that the tick thread quits and joins.
 */
Animation::~Animation()
{
    this->stop();
}

/**
 * Start the animation tick thread.
 */
void Animation::start()
{
    this->stop();

    this->running = true;

    this->tick_thread = std::shared_ptr<std::thread>( new std::thread(Animation::tick_loop, this) );
}

/**
 * Stop the animation tick thread.
 */
void Animation::stop() {
    this->running = false;
    this->loaded = false;

    if (this->tick_thread && this->tick_thread.get()->joinable()) {
        this->tick_thread.get()->join();
    }
}

/**
 * Return whether the tick thread is runnin or not.
 *
 * @return bool
 */
bool Animation::check_running() const
{
    return this->running;
}

bool Animation::check_loaded() const
{
    return this->loaded;
}

/**
 * Return the tick rate for this animation.
 *
 * @return int
 */
int Animation::get_tick_rate() const
{
    return this->tick_rate;
}

/**
 * Default render behaviour simply prints the frame time and rate.
 */
bool Animation::on_render()
{
    GLuint64 current_time = Utilities::get_micro_time();
    this->frame_count++;
    if (current_time - this->last_frame_time >= 1000000) {
        std::cout << "Frame time: " << 1000000./static_cast<GLfloat>(this->frame_count) << " FPS: " << this->frame_count << std::endl;
        this->frame_count = 0;
        this->last_frame_time = current_time;
    }
}

/**
 * Perform functions in the tick thread that should occur before we call ourselves "loaded".
 */
void Animation::on_load()
{
    usleep(300000);
    this->loaded = true;
}

/**
 * Compute a tick
 */
void Animation::on_tick(GLuint64 time_delta)
{
    //Tick all the objects
    for (
        std::map< std::string, std::unique_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
        it != this->objects.end();
        ++it
    ) {
        it->second->on_tick(time_delta);
    }
}

/**
 * Run a loop that calls the animations on_tick method.
 * Quit when check_running() returns false.
 */
void Animation::tick_loop(Animation *animation)
{
    animation->on_load();

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
        throw std::runtime_error("Requested non existant object.");
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

/**
 * Remove all object references.
 */
void Animation::clear_objects()
{
    this->objects.clear();
}
