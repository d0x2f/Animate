#include <iostream>
#include <GL/glew.h>

#include "Area.hh"
#include "Textures.hh"
#include "../Context.hh"

using namespace Animate::GL;

/**
 * Constructor
 */
Area::Area(Context *context) : context(context)
{
    //Add ourself to context
    this->context->set_gl_area(this);

    //Create a texture manager
    new Textures(this->context);

    //Enable depth buffer
    set_has_depth_buffer(true);
}

/**
 * Destructor
 */
Area::~Area()
{
}

/**
 * @override
 * Initialises glew and prints OpenGL version information.
 */
void Area::on_realize()
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
