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

    //Set puzzle values
    this->grid_size = 3;

    std::vector<size_t> initial_position = taquin_generate_vector(this->grid_size);
    this->move_sequence = taquin_solve(initial_position, this->grid_size);

    //Initialise cat tiles
    Tile *tile;
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        if (initial_position[i] == 0) {
            this->zero_position = i;
            continue;
        }
        tile = new Tile(Point(), Scale(1., 1., 1.));
        tile->initialise(
            this->shader.get(),
            this->context->get_textures()->get_texture("/Animate/data/Cat/lily.jpg"),
            initial_position[i], //board position
            this->grid_size  //Grid size
        );
        tile->set_board_position(Position(i%this->grid_size, i/this->grid_size));

        this->add_object("tile"+std::to_string(i), tile);
        this->tile_position_map.insert(std::pair<int, Tile *> (i, tile));
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
    Animation::on_render(gl_context);

    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, this->grid_size, 0, this->grid_size, 0, 1);

    this->shader.get()->set_matrices(model_matrix, view_matrix, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);
        glClearColor(
            0.7,
            0.7,
            0.7,
            1.0
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
    bool in_motion = false;
    for (
        std::map< std::string, std::unique_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
        it != this->objects.end();
        ++it
    ) {
        it->second->on_tick(time_delta);
        in_motion |= ((Tile *)it->second.get())->is_moving();
    }

    //If tiles are moving, skip
    if (in_motion | this->move_sequence.empty()) {
        return;
    }

    TaquinSolve::Moves move = this->move_sequence.front();
    this->move_sequence.pop();

    int from_position = this->zero_position;
    int to_position = 0;
    switch (move) {
        case TaquinSolve::Moves::UP:
            to_position = from_position - this->grid_size;
            break;

        case TaquinSolve::Moves::DOWN:
            to_position = from_position + this->grid_size;
            break;

        case TaquinSolve::Moves::LEFT:
            to_position = from_position - 1;
            break;

        case TaquinSolve::Moves::RIGHT:
            to_position = from_position + 1;
            break;
    }

    std::map<int, Tile *>::iterator it = this->tile_position_map.find(to_position);
    Tile *tile = it->second;
    it->second->move_to_board_position(Position(
        from_position % this->grid_size,
        from_position / this->grid_size
    ));
    this->tile_position_map.erase(it);
    this->tile_position_map.insert(std::pair<int, Tile *>(from_position, tile));

    this->zero_position = to_position;
}
