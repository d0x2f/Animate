#include <gtkmm.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>

#include "Cat.hh"

using namespace Animate;

/**
 * Constructor.
 * Seed the RNG.
 */
Cat::Cat(GLArea *gl_area) : gl_area(gl_area)
{
    srand(time(NULL));
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
            this->colour.r,
            this->colour.g,
            this->colour.b,
            this->colour.a
        );
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    return true;
}

/**
 * Compute a tick
 */
void Cat::on_tick()
{
    std::lock_guard<std::mutex> guard(this->colour_mutex);

    float max = static_cast <float> (RAND_MAX);
    float r = static_cast <float> (rand()) / max;
    float b = static_cast <float> (rand()) / max;
    float g = static_cast <float> (rand()) / max;

    this->colour = Colour(r,g,b);

    //Now that the geometry has changed, trigger a render
    this->gl_area->queue_render();
}