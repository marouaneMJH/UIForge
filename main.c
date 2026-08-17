
#include "./include/builder.h"


// Activate callback for GtkApplication
static void activate(GtkApplication *app, gpointer user_data)
{
    root_app = app;

    // // Create a new window
    View *root_view = build_app(app, NULL, INDEX_TXT);
    GtkWidget *window = root_view->widget;


    ViewConfig *view_conf;
    SAFE_ALLOC(view_conf, ViewConfig, 1);

    View *commands_container = find_view_by_id("commands_container", root_view);
    if (!commands_container)
    {
        g_print("Error: ==> Cannot find the commands container\n");
        return;
    }

    add_command("Create button", 10, 10, "button", commands_container, root_view);
    add_command("Create notebook", 100, 10, "notebook", commands_container, root_view);


    strcpy(view_conf->signal.sig_handler, "sig_open_import_dialog");
    add_custom_command(view_conf, "Print hello", 10, 50, commands_container, root_view);
  
    show_window(window);
}

// Main function
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // Create a new application instance
    app = gtk_application_new("gtk.root.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);

    return status;
}
