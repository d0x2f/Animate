#include <gtkmm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    this->reset_puzzle_flag = false;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Cat::initialise()
{
    std::cout << "cat init" << std::endl;

    //Set shaders
    this->shader = std::shared_ptr<Shader>(new Shader(this->context, "/Animate/data/Cat/shader.frag", "/Animate/data/Cat/shader.vert"));
    this->shader.get()->initialise();
}

void Cat::reset_puzzle()
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    //Clear tiles
    this->clear_objects();
    this->tile_position_map.clear();

    //Set puzzle values
    std::string texture_name = "/Animate/data/Cat/" + std::to_string(this->texture_index++) + ".jpg";
    this->texture_index %= 7;

    //Initialise cat tiles
    Tile *tile;
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        if (this->initial_position[i] == 0) {
            this->zero_position = i;
            continue;
        }
        tile = new Tile(Point(), Scale(1., 1., 1.));
        tile->initialise(
            this->shader.get(),
            this->context->get_textures()->get_texture(texture_name),
            this->initial_position[i], //board position
            this->grid_size  //Grid size
        );
        tile->set_board_position(Position(i%this->grid_size, i/this->grid_size));

        this->add_object("tile"+std::to_string(i), tile);
        this->tile_position_map.insert(std::pair<int, Tile *> (i, tile));
    }
}

/**
 * Render a frame.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Cat::on_render()
{
    Animation::on_render();

    if (this->reset_puzzle_flag) {
        this->reset_puzzle();
        this->reset_puzzle_flag = false;
    }

    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, this->grid_size, 0, this->grid_size, 0, 1);

    this->shader->set_matrices(model_matrix, view_matrix, projection_matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(
        0.7,
        0.7,
        0.7,
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
void Cat::on_tick(GLuint64 time_delta)
{
    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);

        Animation::on_tick(time_delta);

        //Check if any tiles are moving
        bool in_motion = false;
        for (
            std::map< std::string, std::shared_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
            it != this->objects.end();
            ++it
        ) {
            in_motion |= ((Tile *)it->second.get())->is_moving();
        }

        //If tiles are moving, skip
        if (in_motion) {
            return;
        }
    }

    if (this->move_sequence.empty()) {
        this->initial_position = taquin_generate_vector(this->grid_size);
        this->move_sequence = taquin_solve(this->initial_position, this->grid_size);

        this->reset_puzzle_flag = true;
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

    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);

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
}
