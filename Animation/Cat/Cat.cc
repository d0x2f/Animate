#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Cat.hh"
#include "../Utilities.hh"
#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../GL/Shader.hh"

using namespace Animate::Animation;
using namespace Animate::Geometry;
using namespace Animate::GL;

/**
 * Constructor.
 * Seed the RNG.
 */
Cat::Cat(Context *context) : Animation(context)
{
    this->tick_rate = 60;

    srand(time(NULL));

    some_quad = std::unique_ptr<Quad>(new Quad(Point(), Scale(5., 5., 1.)));
}

/**
 * Perform initialisations.
 */
void Cat::on_realise()
{
    //Set shaders
    this->shader = std::unique_ptr<Shader>(new Shader(this->context, "/Animate/data/Cat/shader.frag", "/Animate/data/Cat/shader.vert"));
    this->shader.get()->initialise();

    //Initialise quad
    some_quad.get()->initialise(
        this->shader.get(),
        this->context->get_textures()->get_texture("/Animate/data/Cat/lily.jpg")
    );

    GLfloat max = static_cast <GLfloat> (RAND_MAX);
    this->some_quad_direction = Vector3(
        ( static_cast <GLfloat> (rand()) / max ) - 0.5,
        ( static_cast <GLfloat> (rand()) / max ) - 0.5
    ).normalise();

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
    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 10, 0, 10, 0, 1);

    this->shader.get()->set_matrices(model_matrix, view_matrix, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);
        glClearColor(
            this->colour.x,
            this->colour.y,
            this->colour.z,
            this->colour.w
        );

        some_quad.get()->draw(model_matrix);
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
void Cat::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    //Move at 1 unit per second in the direction of the current vector
    GLfloat move_factor = static_cast <GLfloat> (time_delta) / 100000.;
    Point pos = this->some_quad->get_position() + (this->some_quad_direction * move_factor);
    Point diff = (this->some_quad_direction * move_factor);

    //If this would be out of bounds, then reflect
    Point bottom_left = pos;
    Point top_right = pos + Vector3(5.,5.);
    if (
        bottom_left.is_out_of_bounds(Vector2(), Vector2(10, 10)) ||
        top_right.is_out_of_bounds(Vector2(), Vector2(10, 10))
    ) {
        if (bottom_left.x < 0 || top_right.x > 10)
            this->some_quad_direction.x *= -1;

        if (bottom_left.y < 0 || top_right.y > 10)
            this->some_quad_direction.y *= -1;

        pos = this->some_quad->get_position() + (this->some_quad_direction * move_factor);

        //Change bg colour
        GLfloat max = static_cast <GLfloat> (RAND_MAX);
        GLfloat r = static_cast <GLfloat> (rand()) / max;
        GLfloat b = static_cast <GLfloat> (rand()) / max;
        GLfloat g = static_cast <GLfloat> (rand()) / max;

        this->colour = Colour(r,g,b);
    }

    //Set new position
    this->some_quad->set_position(pos);

    //Now that the geometry has changed, trigger a render
    /*GL::Area *gl_area = this->context->get_gl_area();
    if (gl_area != NULL) {
        gl_area->queue_render();
    }*/
}
