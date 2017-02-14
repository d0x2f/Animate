#include <gtk/gtk.h>
#include <GL/glew.h>

// Hold init data for GTK signals:
struct signal {
    const gchar     *signal;
    GCallback       handler;
    GdkEventMask    mask;
};

static gboolean on_render(GtkGLArea *glarea, GdkGLContext *context)
{
    // Clear canvas:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Don't propagate signal:
    return TRUE;
}

static void connect_signals(GtkWidget *widget, struct signal *signals, size_t members)
{
    for (size_t i = 0; i < members; i++) {
        gtk_widget_add_events(widget, signals[i].mask);
        g_signal_connect(widget, signals[i].signal, signals[i].handler, NULL);
    }
}

static void connect_window_signals(GtkWidget *window)
{
    struct signal signals[] = {
        { "destroy",                G_CALLBACK(gtk_main_quit),      (GdkEventMask) 0}
    };

    connect_signals(window, signals, sizeof(signals) / sizeof(signals[0]));
}

static void connect_glarea_signals(GtkWidget *glarea)
{
    struct signal signals[] = {
        { "render",                 G_CALLBACK(on_render),          (GdkEventMask) 0}
    };

    connect_signals(glarea, signals, sizeof(signals) / sizeof(signals[0]));
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window, *gl_area;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Animate");
    gtk_window_set_default_size(GTK_WINDOW(window), 720, 1280);

    gl_area = gtk_gl_area_new();

    gtk_container_add(GTK_CONTAINER(window), gl_area);

    gtk_widget_show_all(window);
}

int gui_run(int argc, char **argv)
{
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("dog.dyl.animate", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}