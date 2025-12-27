// #include "./../../../include/widgets/dialogs/dialog.h"
#include "./../../../include/widgets/dialogs/dialog.h"
#include "./../../../include/widgets/view/signals.h"

ViewConfig *configure_dialog_property(DialogConfig *dialog_config, ViewConfig *view_config, gchar *property, gchar *value, int *status)
{
    if (!dialog_config || !property || !value)
    {
        *status = 0;
        return NULL;
    }

    if (g_strcmp0(property, "title") == 0)
        strcpy(dialog_config->title, value);

    if (g_strcmp0(property, "icon_path") == 0)
        strcpy(dialog_config->icon_path, value);

    if (g_strcmp0(property, "bg_color") == 0)
        strcpy(dialog_config->bg_color, value);

    if (g_strcmp0(property, "text_color") == 0)
        strcpy(dialog_config->text_color, value);

    if (g_strcmp0(property, "is_modal") == 0)
        dialog_config->is_modal = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    if (g_strcmp0(property, "has_header") == 0)
        dialog_config->has_header = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    if (g_strcmp0(property, "width") == 0)
        dialog_config->dimensions.width = atoi(value);

    if (g_strcmp0(property, "height") == 0)
        dialog_config->dimensions.height = atoi(value);

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_dialog_config(FILE *index, DialogConfig *dialog_config)
{
    if (!index || !dialog_config)
        return NULL;

    // Allocate memory for the view dialog_config
    ViewConfig *view_config = NULL;
    SAFE_ALLOC(view_config, ViewConfig, 1);
    DFEAULT_VIEW_CONFIG(view_config);

    // Store the property and value of the tag
    gchar *property = NULL;
    gchar *value = NULL;

    // Read the tag character by character
    gchar c;
    while ((c = fgetc(index)) != '>')
    {
        /* If the character is a letter then go back one character
            Because when the tag is readed the cursor will start with the first letter in the property and it will be lost */
        if (is_character(c))
            fseek(index, -1, SEEK_CUR);

        int status = -1;

        // Read the property of the tag
        property = read_property(index, &status);

        // If the all properties are readed then break the loop and return the view id and pass the properties to the dialog dialog_config
        if (status == 2)
            return view_config;

        // If the property is readed then read the value of the property
        else if (status == 1 && property)
        {
            // Read the value of the property
            value = read_value(index, &status);
            if (status == 1 && value)
            {
                if (g_strcmp0(property, "id") == 0) // Store the view id
                {
                    strcpy(view_config->view_id, value);
                    free(property);
                }
                else
                {
                    // Apply the property value to the dialog dialog_config
                    int config_status;
                    view_config = configure_dialog_property(dialog_config, view_config, property, value, &config_status);
                    if (!config_status)
                    {
                        printf("Error configuring the property %s\n", property);
                    }
                    free(value);
                    free(property);
                }
            }
        }
    }

    return view_config;
}

static void dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{

    switch (response_id)
    {
    case GTK_RESPONSE_OK:
        g_print("User clicked OK.\n");
        break;

    case GTK_RESPONSE_CANCEL:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_DELETE_EVENT:
        g_print("User clicked Exiting the box.\n");
        gtk_widget_destroy(GTK_WIDGET(dialog)); // Close the dialog
        break;

    case GTK_RESPONSE_YES:
        g_print("User clicked Yes.\n");
        break;

    case GTK_RESPONSE_NO:
        g_print("User clicked No.\n");
        gtk_widget_destroy(GTK_WIDGET(dialog)); // Close the dialog
        break;

    case 100: // Custom response
        g_print("Custom response 100 triggered.\n");
        break;

    default:
        g_print("Unknown response ID: %d\n", response_id);
        break;
    }
}

GtkWidget *create_dialog(DialogConfig dialog_config)
{
    // Use the provided parent window or fallback to the global root view
    GtkWindow *parent = dialog_config.parent ? dialog_config.parent : GTK_WINDOW(root_view_global->widget);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        dialog_config.title,
        parent,
        dialog_config.is_modal ? GTK_DIALOG_MODAL : 0,
        NULL);

    // Make the dialog resizable
    gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);

    // Allow maximize and minimize buttons
    gtk_window_set_deletable(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_type_hint(GTK_WINDOW(dialog), GDK_WINDOW_TYPE_HINT_NORMAL);

    // Set the header bar if needed
    if (dialog_config.has_header)
        set_header_bar(dialog, dialog_config.title, dialog_config.icon_path);

    // Set default size
    gtk_window_set_default_size(GTK_WINDOW(dialog), dialog_config.dimensions.width, dialog_config.dimensions.height);

    // Set background color if provided
    widget_set_colors(dialog, dialog_config.bg_color, dialog_config.text_color);

    // Connect the response signal
    g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(dialog_response), NULL);

    return dialog;
}

void show_dialog(GtkWidget *dialog)
{
    /* Run the dialog */
    if (dialog)
    {
        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
    }
}

void destroy_dialog(GtkWidget *dialog)
{
    gtk_widget_destroy(dialog);
}