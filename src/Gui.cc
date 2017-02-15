#include <GL/glew.h>
#include <iostream>

#include "Gui.hh"
#include "animations/Cat.hh"

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
    gl_area = new GLArea();
    add(*gl_area);

    //Create animation and connect it up
    Cat *animation = new Cat();
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
}
