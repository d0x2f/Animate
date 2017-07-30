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
}

/**
 * Destructor.
 */
Animation::~Animation()
{
}

/**
 * Perform functions in the tick thread that should occur before we call ourselves "loaded".
 */
void Animation::on_load()
{
    usleep(300000);
    this->loaded = true;
}

void Animation::unload()
{
    this->loaded = false;
}

bool Animation::check_loaded()
{
    return this->loaded;
}

/**
 * Compute a tick
 */
void Animation::on_tick(GLuint64 time_delta)
{
    //Tick all the objects
    for(auto const& object: this->objects) {
        object.second->on_tick(time_delta);
        object.second->add_to_scene();
    }
}

/**
 * Fetch an object from the list.
 *
 * @param name      The name of the object.
 *
 * @return the object.
 */
std::weak_ptr<Object> Animation::get_object(std::string name)
{
    std::map< std::string, std::shared_ptr<Object::Object> >::iterator it;
    it = this->objects.find(name);
    if (it == this->objects.end()) {
        throw std::runtime_error("Requested non existant object.");
    }
    return it->second;
}

/**
 * Add an object to the list.
 *
 * @param name      The name of the object.
 * @param object    The object to add.
 */
void Animation::add_object(std::string name, Object::Object *object)
{
    this->objects.insert( std::pair<std::string, std::shared_ptr<Object::Object> >(name, object));
}

/**
 * Remove all object references.
 */
void Animation::clear_objects()
{
    this->objects.clear();
}
