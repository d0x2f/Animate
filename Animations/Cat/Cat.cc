#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Cat.hh"
#include "../Utilities.hh"
#include "../GL/Definitions.hh"
#include "../GL/Matrix.hh"
#include "../GL/Shader.hh"

using namespace Animate;

/**
 * Constructor.
 * Seed the RNG.
 */
Cat::Cat(Context *context) : Animation(context)
{
    this->tick_rate = 5;

    srand(time(NULL));

    some_quad = std::unique_ptr<GL::Quad>(new GL::Quad(GL::Point(), GL::Size(20., 20.), GL::Colour(1.)));
}

/**
 * Perform initialisations.
 */
void Cat::on_realise()
{
    //Initialise quad
    some_quad.get()->initialise();

    //Set shaders
    this->shader = std::unique_ptr<GL::Shader>(new GL::Shader(this->context, "/Animate/data/Cat/shader.frag", "/Animate/data/Cat/shader.vert"));
    this->shader.get()->initialise();

    //Set matrices
    GL::Matrix model_matrix(
        GL::Vector4( 1., 0., 0., 0.),
        GL::Vector4( 0., 1., 0., 0.),
        GL::Vector4( 0., 0., 1., 0.),
        GL::Vector4( 0., 0., 0., 1.)
    );

    //Look at
    GL::Matrix view_matrix = GL::Matrix::look_at(
        GL::Vector3(0., 0., 1.), // Eye
        GL::Vector3()            // Center (looking at)
    );

    //Ortho
    GL::Matrix projection_matrix = GL::Matrix::orthographic(0, 6, 0, 10, 0, 1);

    this->shader.get()->set_matrices(model_matrix, view_matrix, projection_matrix);

    //Start tick thread
    this->run();
}

/**
 * Render a frame.
 *
 * @param gl_context    GDK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Cat::on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context)
{
    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->colour_mutex);
        glClearColor(
            this->colour.x,
            this->colour.y,
            this->colour.z,
            this->colour.w
        );
    }

    glClear(GL_COLOR_BUFFER_BIT);

    this->shader.get()->use();

    some_quad.get()->draw();

    glUseProgram(0);

    glFlush();

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cerr << "OPENGL ERROR: " << gluErrorString(error) << "(" << error << ")" << std::endl;

    return true;
}

/**
 * Compute a tick
 */
void Cat::on_tick()
{
    std::lock_guard<std::mutex> guard(this->colour_mutex);

    GLfloat max = static_cast <GLfloat> (RAND_MAX);
    GLfloat r = static_cast <GLfloat> (rand()) / max;
    GLfloat b = static_cast <GLfloat> (rand()) / max;
    GLfloat g = static_cast <GLfloat> (rand()) / max;

    this->colour = GL::Colour(r,g,b);

    //Now that the geometry has changed, trigger a render
    GL::Area *gl_area = this->context->get_gl_area();
    if (gl_area != NULL) {
        gl_area->queue_render();
    }
}
