#include <iostream>
#include <GL/glew.h>

#include "GLArea.hh"

using namespace Animate;

/**
 * Constructor
 */
GLArea::GLArea()
{
}

/**
 * Destructor
 */
GLArea::~GLArea()
{
}

/**
 * @override
 * Initialises glew and prints OpenGL version information.
 */
void GLArea::on_realize()
{
    Gtk::GLArea::on_realize();

    //Activate this area
    make_current();

    //Initialise glew
    glewInit();

    //Print version information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    //Enable depth buffer
    set_has_depth_buffer(TRUE);
}