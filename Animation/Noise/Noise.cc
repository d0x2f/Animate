#include <gtkmm.h>
#include <iostream>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>

#include "Noise.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../VK/Circle.hh"
#include "../../VK/Shader.hh"

using namespace Animate::Animation::Noise;
using namespace Animate::Geometry;
using namespace Animate::VK;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Noise::Noise(Context *context) : Animation::Animation(context)
{
    this->tick_rate = 1;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Noise::initialise()
{
    //Set shaders
    this->shader = std::shared_ptr<Shader>(new Shader(this->context, "/Animate/data/Noise/shader.frag", "/Animate/data/Noise/shader.vert"));
    this->shader.get()->initialise();

    //Add a Quad
    Object::Object *object = new Object::Object();
    Quad *quad = new Quad();
    quad->initialise(
        this->shader.get(),
        new Texture()
    );
    object->add_component(quad);
    this->add_object("quad", object);
}

/**
 * Render a frame.
 *
 * @param gl_context    GDK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Noise::on_render()
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

        this->shader->set_uniform("random_seed", static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

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
void Noise::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    Animation::on_tick(time_delta);
}
