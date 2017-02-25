#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Cat.hh"
#include "Object/Tile.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../GL/Shader.hh"

using namespace Animate::Animation::Cat;
using namespace Animate::Animation::Cat::Object;
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

    //Initialise cat tile
    Tile *tile;
    for (int i = 0; i < 4; i++) {
        tile = new Tile(Point(), Scale(1., 1., 1.));
        tile->initialise(
            this->shader.get(),
            this->context->get_textures()->get_texture("/Animate/data/Cat/lily.jpg")
        );

        tile->set_reflect_callback([&]() {
            //Change bg colour
            GLfloat max = static_cast <GLfloat> (RAND_MAX);
            GLfloat r = static_cast <GLfloat> (rand()) / max;
            GLfloat b = static_cast <GLfloat> (rand()) / max;
            GLfloat g = static_cast <GLfloat> (rand()) / max;

            this->colour = Colour(r,g,b);
        });

        this->add_object("tile"+std::to_string(i), tile);
    }

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
void Cat::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    //Tick every object
    for (
        std::map< std::string, std::unique_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
        it != this->objects.end();
        ++it
    ) {
        it->second->on_tick(time_delta);
    }

    //Now that the geometry has changed, trigger a render
    /*GL::Area *gl_area = this->context->get_gl_area();
    if (gl_area != NULL) {
        gl_area->queue_render();
    }*/
}
