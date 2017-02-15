#include <gtk/gtk.h>
#include <GL/glew.h>
#include <iostream>

// GTK signal data
struct signal {
    const gchar     *signal;
    GCallback       handler;
    GdkEventMask    mask;
};

/**
 * Render a frame.
 *
 * @param gl_area       GTK OpenGL area reference.
 * @param gl_context    GTK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
static gboolean on_render(GtkGLArea *gl_area, GdkGLContext *gl_context)
{
    // Clear canvas
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Don't bubble the signal
    return TRUE;
}

/**
 * Triggered when the OpenGL context is created.
 * Initialisation functions should go here.
 *
 * @param gl_area       GTK OpenGL area reference.
 *
 * @return False so that the signal can bubble.
 */
static gboolean on_realise(GtkGLArea *gl_area)
{
    //Activate this area
    gtk_gl_area_make_current(gl_area);

    //Initialise glew
    glewInit();

    //Print version information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    //Enable depth buffer
    gtk_gl_area_set_has_depth_buffer(gl_area, TRUE);

    // Get frame clock
    GdkGLContext *gl_context = gtk_gl_area_get_context(gl_area);
    GdkWindow *gl_window = gdk_gl_context_get_window(gl_context);
    GdkFrameClock *frame_clock = gdk_window_get_frame_clock(gl_window);

    // Connect update signal
    g_signal_connect_swapped(
        frame_clock,
        "update",
        G_CALLBACK(gtk_gl_area_queue_render),
        gl_area
    );

    // Start updating
    gdk_frame_clock_begin_updating(frame_clock);

    return FALSE;
}

/**
 * Connect the array of given signals.
 *
 * @param widget       The widget to connect from.
 * @param signals      An array of signal and slot settings to connect.
 * @param members      The number of signals contained in the signals parameter.
 */
static void connect_signals(GtkWidget *widget, struct signal *signals, size_t members)
{
    for (size_t i = 0; i < members; i++) {
        gtk_widget_add_events(widget, signals[i].mask);
        g_signal_connect(widget, signals[i].signal, signals[i].handler, NULL);
    }
}

/**
 * Connect window signals.
 * Contains a list of signal connection details.
 *
 * @param window       The window to receive signals from.
 */
static void connect_window_signals(GtkWidget *window)
{
    struct signal signals[] = {
    };

    connect_signals(window, signals, sizeof(signals) / sizeof(signals[0]));
}

/**
 * Connect OpenGL signals.
 * Contains a list of signal connection details.
 *
 * @param window       The OpenGL area to receive signals from.
 */
static void connect_glarea_signals(GtkWidget *gl_area)
{
    struct signal signals[] = {
        { "render",                 G_CALLBACK(on_render),           (GdkEventMask) 0},
        { "realize",                G_CALLBACK(on_realise),          (GdkEventMask) 0}
    };

    connect_signals(gl_area, signals, sizeof(signals) / sizeof(signals[0]));
}

/**
 * Slot for the GTK application activation signal.
 * Creates a window, OpenGL area and displays them.
 * Initiates signal connection functions.
 *
 * @param app        The GTK application that has been activated.
 * @param user_data  Data given via command line arguments.
 */
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window, *gl_area;

    //Create a window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Animate");
    gtk_window_set_default_size(GTK_WINDOW(window), 720, 1280);

    //Create a gl area and add it to the window
    gl_area = gtk_gl_area_new();
    gtk_container_add(GTK_CONTAINER(window), gl_area);

    //Connect signals
    connect_window_signals(window);
    connect_glarea_signals(gl_area);

    //Show the window
    gtk_widget_show_all(window);
}

/**
 * Create a GTK application, connect the activation signal and run it.
 *
 * @param argc        The number of command line tokens given.
 * @param argv        An array of command line tokens.
 *
 * @return The program result.
 */
int gui_run(int argc, char **argv)
{
    GtkApplication *app;
    int status;
    
    //Create the GTK application
    app = gtk_application_new("dog.dyl.animate", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}