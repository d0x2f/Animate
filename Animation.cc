#include <iostream>

#include "Animation.hh"

using namespace Animate;

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

/**
 * Run a loop that calls the animations on_tick method.
 * Quit when check_running() returns false.
 */
void Animation::tick_loop(Animation *animation)
{
    while (animation->check_running()) {
        animation->on_tick();
        usleep(1000000. / animation->get_tick_rate()); 
    }
}