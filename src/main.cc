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
    //Create the GTK application
    auto app = Gtk::Application::create(argc, argv, "dog.dyl.animate");
    Animate::Gui gui;
    return app->run(gui);
}