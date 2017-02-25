#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Cat.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../GL/Shader.hh"

using namespace Animate::Animation;
using namespace Animate::Geometry;
using namespace Animate::GL;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Cat::Cat(Context *context) : Animation::Animation(context)
{
    this->tick_rate = 60;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Cat::on_realise()
{
    //Set shaders
    this->shader = std::unique_ptr<Shader>(new Shader(this->context, "/Animate/data/Cat/shader.frag", "/Animate/data/Cat/shader.vert"));
    this->shader.get()->initialise();

    //Initialise cat pic
    Quad *quad = new Quad(Point(), Scale(1., 1., 1.));
    quad->initialise(
        this->shader.get(),
        this->context->get_textures()->get_texture("/Animate/data/Cat/lily.jpg")
    );
    Object::Object *cat = new Object::Object(Point(), Scale(1., 1., 1.));
    cat->add_component(quad);
    cat->initialise();
    this->add_object("cat", cat);

    GLfloat max = static_cast <GLfloat> (RAND_MAX);
    this->cat_direction = Vector3(
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

        //Draw every object
        for (
            std::map< std::string, std::unique_ptr<Object::Object> >::iterator it = this->objects.begin();
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
void Cat::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    Object::Object *cat = this->get_object("cat");

    //Move at 1 unit per second in the direction of the current vector
    GLfloat move_factor = static_cast <GLfloat> (time_delta) / 100000.;
    Point pos = cat->get_position() + (this->cat_direction * move_factor);
    Point diff = (this->cat_direction * move_factor);

    //If this would be out of bounds, then reflect
    Point bottom_left = pos;
    Point top_right = pos + Vector3(1., 1.);
    if (
        bottom_left.is_out_of_bounds(Vector2(), Vector2(10, 10)) ||
        top_right.is_out_of_bounds(Vector2(), Vector2(10, 10))
    ) {
        if (bottom_left.x < 0 || top_right.x > 10)
            this->cat_direction.x *= -1;

        if (bottom_left.y < 0 || top_right.y > 10)
            this->cat_direction.y *= -1;

        pos = cat->get_position() + (this->cat_direction * move_factor);

        //Change bg colour
        GLfloat max = static_cast <GLfloat> (RAND_MAX);
        GLfloat r = static_cast <GLfloat> (rand()) / max;
        GLfloat b = static_cast <GLfloat> (rand()) / max;
        GLfloat g = static_cast <GLfloat> (rand()) / max;

        this->colour = Colour(r,g,b);
    }

    //Set new position
    cat->set_position(pos);

    //Now that the geometry has changed, trigger a render
    /*GL::Area *gl_area = this->context->get_gl_area();
    if (gl_area != NULL) {
        gl_area->queue_render();
    }*/
}
