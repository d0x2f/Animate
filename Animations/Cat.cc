#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Cat.hh"
#include "../Utilities.hh"
#include "../GL/Definitions.hh"
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


void Cat::on_realise()
{
    //Initialise quad
    some_quad.get()->initialise();

    //Set shaders
    GL::Shader shader(this->context, "/Cat/shader.frag", "/Cat/shader.vert");
    shader.initialise();
    shader.use();

    GL::Vector3 eye(0., 0., -3.);
    GL::Vector3 f = GL::Vector3() - eye;
    GL::Vector3 up(0., 1., 0.);

    f = f.normalise();

    GL::Vector3 s = f.cross(up);
    GL::Vector3 u = s.normalise().cross(f);

    GL::Matrix44 model_view_matrix(
        GL::Vector4( s.x,  s.y,  s.z, -eye.x),
        GL::Vector4( u.x,  u.y,  u.z, -eye.y),
        GL::Vector4(-f.x, -f.y, -f.x, -eye.z),
        GL::Vector4(   0.,  0.,   0.,     1.)
    );

    GLfloat left = -0.346410162,
            right = 0.346410162,
            bottom = 0.57735026972,
            top = -0.57735026972,
            near = 1.,
            far = 10.;

    GL::Matrix44 projection_matrix(
        GL::Vector4(2*near / (right - left), 0., (right+left)/(right-left), 0.),
        GL::Vector4(0., 2*near / (top - bottom), (top+bottom)/(top-bottom), 0.),
        GL::Vector4(0., 0., -((far+near)/(far-near)), -((2*far*near)/(far-near))),
        GL::Vector4(0., 0., -1, 0.)
    );

    shader.set_matrices(model_view_matrix, projection_matrix);

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
        /*glClearColor(
            this->colour.x,
            this->colour.y,
            this->colour.z,
            this->colour.w
        );*/
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    some_quad.get()->draw();

    glFlush();
    
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cerr << "init OPENGL ERROR: " << gluErrorString(error) << "(" << error << ")" << std::endl;

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
