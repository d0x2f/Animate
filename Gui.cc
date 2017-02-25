#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>

#include "Gui.hh"
#include "Animation/Cat/Cat.hh"

using namespace Animate;
using namespace Animate::Animation;

/**
 * Sets the window size and title.
 * Adds a GLArea to the window.
 * Starts the cat animation.
 */
Gui::Gui()
{
    set_default_size(1024, 1024);
    set_title("Animate");

    //Create context object
    this->context = std::unique_ptr<Context>( new Context() );

    //Create a gl area and add it to the window
    new GL::Area(this->context.get());

    add(*(this->context.get()->get_gl_area()));

    //Create animation and connect it up
    Animation::Cat::Cat *animation = new Animation::Cat::Cat(this->context.get());
    set_animation(animation);

    //Show the childrem
    show_all_children();
}

/**
 * Destructor
 */
Gui::~Gui()
{
}

/**
 * Set the currently running animation.
 * Unbinds existing render signals and connects the given animation.
 *
 * @param animation        The animation to play.
 */
void Gui::set_animation(Animation::Animation *animation)
{
    //Triggerd an existing animations destructor
    this->current_animation.reset(animation);

    //Connect the render signal
    this->context.get()->get_gl_area()->signal_render().connect(sigc::mem_fun(this->current_animation.get(), &Animation::Animation::on_render));

    //Connect the realise signal
    this->context.get()->get_gl_area()->signal_realize().connect(sigc::mem_fun(this->current_animation.get(), &Animation::Animation::on_realise));
}
