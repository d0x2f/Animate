#include <gtkmm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    this->tick_rate = 120;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Modulo::initialise()
{
    //Set shaders
    this->shader = std::shared_ptr<Shader>(new Shader(this->context, "/Animate/data/Modulo/shader.frag", "/Animate/data/Modulo/shader.vert"));
    this->shader->initialise();

    //Add a circle
    Ring *ring = new Ring(Point(0.5,0.5), Scale(.8,.8));
    ring->initialise(
        this->shader.get()
    );
    this->add_object("ring", ring);
}

/**
 * Render a frame.
 *
 * @param gl_context    GDK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Modulo::on_render()
{
    Animation::on_render();

    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 1, 0, 1, 0, 1);

    this->shader->set_matrices(model_matrix, view_matrix, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(
        0.,
        0.,
        0.,
        1.0
    );

    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);

        //Draw every object
        for (
            std::map< std::string, std::shared_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
            it != this->objects.end();
            ++it
        ) {
            it->second->draw(model_matrix);
        }
    }

    glFlush();

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
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    Animation::on_tick(time_delta);
}
