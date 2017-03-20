#include <gtkmm.h>
#include <iostream>
#include <stdexcept>

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
        auto app = Gtk::Application::create(argc, argv, "dog.dyl.animate");
        Animate::Gui gui;
        gui.start_loop();
    } catch (std::string error) {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    } catch (std::string error) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
