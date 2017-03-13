#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Modulo.hh"
#include "Object/Ring.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../GL/Circle.hh"
#include "../../GL/Shader.hh"

using namespace Animate::Animation::Modulo;
using namespace Animate::Animation::Modulo::Object;
using namespace Animate::Geometry;
using namespace Animate::GL;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Modulo::Modulo(Context *context) : Animation::Animation(context)
{
    this->tick_rate = 60;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Modulo::on_realise()
{
    //Set shaders
    this->shader = std::unique_ptr<Shader>(new Shader(this->context, "/Animate/data/Modulo/shader.frag", "/Animate/data/Modulo/shader.vert"));
    this->shader.get()->initialise();

    //Add a circle
    Ring *ring = new Ring(Point(0.,0.), Scale(1.,1.));
    ring->initialise(
        this->shader.get()
    );
    this->add_object("ring", ring);

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
bool Modulo::on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context)
{
    Animation::on_render(gl_context);

    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 1, 0, 1, 0, 1);

    this->shader.get()->set_matrices(model_matrix, view_matrix, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(
        0.7,
        0.7,
        0.7,
        1.0
    );

    //Scoped multex lock
    {
        //Draw every object
        for (
            std::map< std::string, std::unique_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
            it != this->objects.end();
            ++it
        ) {
            it->second->draw(model_matrix);
        }
    }

    glFlush();

    GL::Area *gl_area = this->context->get_gl_area();
    if (gl_area != NULL) {
        gl_area->queue_render();
    }

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cerr << "OPENGL ERROR: " << gluErrorString(error) << "(" << error << ")" << std::endl;
    return true;
}

/**
 * Compute a tick
 */
void Modulo::on_tick(GLuint64 time_delta)
{
    //std::lock_guard<std::mutex> guard(this->tick_mutex);
}
