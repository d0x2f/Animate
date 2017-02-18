#include <GL/glew.h>
#include <iostream>
#include <memory>

#include "Gui.hh"
#include "Animations/Cat.hh"

using namespace Animate;

/**
 * Sets the window size and title.
 * Adds a GLArea to the window.
 * Starts the cat animation.
 */
Gui::Gui()
{
    set_default_size(720, 1280);
    set_title("Animate");

    //Create a gl area and add it to the window
    this->gl_area = std::unique_ptr<GL::Area>(new GL::Area());
    add(*(this->gl_area.get()));

    //Create animation and connect it up
    Cat *animation = new Cat(this->gl_area.get());
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
void Gui::set_animation(Animation *animation)
{
    //Triggerd an existing animations destructor
    this->current_animation.reset(animation);

    //Connect the render signal
    this->gl_area->signal_render().connect(sigc::mem_fun(this->current_animation.get(), &Animation::on_render));

    //Start the animation
    animation->run();
}
