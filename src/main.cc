#include <iostream>
#include <stdexcept>

#include "Resources.hh"
#include "Gui.hh"

/**
 * Create a GTK application, connect the activation signal and run it.
 *
 * @param argc        The number of command line tokens given.
 * @param argv        An array of command line tokens.
 *
 * @return The program result.
 */
int main(int argc, char **argv)
{
    try {
        Animate::Resources::initialise();
        Animate::Gui gui;
        gui.start_loops();
    } catch (std::runtime_error const& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
