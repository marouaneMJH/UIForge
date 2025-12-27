#include "./../include/global.h"

View *root_view_global;
View *root_dialog_view_global;
View *root_crud_ui; // debug should change in futur for new versions
GtkApplication *root_app;

View *new_root_view_global;

// inline void free_widget(GtkWidget *widget)
// {
//     if (widget != NULL)
//     {
//         gtk_widget_destroy(widget);
//         widget = NULL; // Avoid dangling pointers
//     }
// }

/////////// /_!_\ Untouchable please //////////////
GtkStyleContext *get_style_provider_context(GtkWidget *widget, const gchar *bg_color, const gchar *color, const gchar *bg_image)
{
    // Create a new CSS provider
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gchar *css_style = NULL;

    // Initialize a GString to build the CSS style
    GString *css_builder = g_string_new(".style { ");

    // Check if any of the color or background image parameters are provided
    if ((color && color[0] != '\0') || (bg_color && bg_color[0] != '\0') || (bg_image && bg_image[0] != '\0'))
    {
        // Append background image to the CSS style if provided
        g_string_append_printf(css_builder, "background-image: url('./assets/images/%s'); ", (bg_image && bg_image[0] != '\0') ? bg_image : "none;");

        // Append background size to the CSS style if background image is provided
        if ((bg_image && bg_image[0] != '\0'))
            g_string_append_printf(css_builder, "background-size: cover; ");

        // Append background color to the CSS style if provided
        if ((bg_color && bg_color[0] != '\0'))
            g_string_append_printf(css_builder, "background-color: %s; ", bg_color);

        // Append text color to the CSS style if provided
        if ((color && color[0] != '\0'))
            g_string_append_printf(css_builder, "color: %s; ", color);

        // Close the CSS style
        g_string_append(css_builder, "}");

        // Convert GString to gchar*
        css_style = g_string_free(css_builder, FALSE);
    }
    else
    {
        // Use a default stylesheet if all arguments are null
        gtk_css_provider_load_from_path(css_provider, "./assets/style.css", NULL);
    }

    // Load the generated CSS style into the CSS provider
    if (css_style)
    {
        GError *error = NULL;
        gtk_css_provider_load_from_data(css_provider, css_style, -1, &error);
        if (error)
        {
            // Print an error message if loading the CSS failed
            g_printerr("Failed to load CSS: %s\n", error->message);
            g_error_free(error);
        }
        g_free(css_style);
    }

    // Get the style context of the widget
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    // Add the CSS provider to the widget's style context
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(css_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Free the CSS provider after use
    g_object_unref(css_provider);

    // Return the style context
    return context;
}

// Function to set both background and text colors for a widget
void widget_set_colors(GtkWidget *widget, const gchar *bg_color, const gchar *color)
{
    if (bg_color)
        g_object_set_data(G_OBJECT(widget), "bg_color", g_strdup(bg_color));
    if (color)
        g_object_set_data(G_OBJECT(widget), "text_color", g_strdup(color));
    // Add a CSS class to the widget's style context with the specified colors
    gtk_style_context_add_class(get_style_provider_context(widget, bg_color, color, NULL), "style");
}

// Function to set a background image for a widget
void widget_set_background_image(GtkWidget *widget, const gchar *bg_image, const gchar *color)
{
    if (bg_image)
        g_object_set_data(G_OBJECT(widget), "bg_image", g_strdup(bg_image));
    if (color)
        g_object_set_data(G_OBJECT(widget), "text_color", g_strdup(color));
    // Add a CSS class to the widget's style context with the specified background image
    gtk_style_context_add_class(get_style_provider_context(widget, NULL, color, bg_image), "style");
}

void widget_set_margins(GtkWidget *widget, Margins margins)
{
    gtk_widget_set_margin_top(widget, margins.top);
    gtk_widget_set_margin_bottom(widget, margins.bottom);
    gtk_widget_set_margin_start(widget, margins.start);
    gtk_widget_set_margin_end(widget, margins.end);
}

void widget_get_margins(GtkWidget *widget, Margins *margins)
{
    if (!margins)
        return;
    margins->top = gtk_widget_get_margin_top(widget);
    margins->bottom = gtk_widget_get_margin_bottom(widget);
    margins->start = gtk_widget_get_margin_start(widget);
    margins->end = gtk_widget_get_margin_end(widget);
}

void widget_set_font_size(GtkWidget *widget, int size)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    // Create CSS string for font size
    gchar *css = g_strdup_printf("* { font-size: %dpt; }", size);

    // Load and apply CSS
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Cleanup
    g_free(css);
    g_object_unref(provider);
}

void widget_set_font_family(GtkWidget *widget, const char *font_family)
{
    // If font_family is NULL, do nothing
    if (font_family == NULL)
    {
        return;
    }

    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    // Create CSS string for font family
    gchar *css = g_strdup_printf("* { font-family: '%s'; }", font_family);

    // Load and apply CSS
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Cleanup
    g_free(css);
    g_object_unref(provider);
}

void set_header_bar(GtkWidget *window, const gchar *title, const gchar *icon_path)
{
    // Create a header bar
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    // gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "menu:close");
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "icon:minimize,maximize,close");

    // Set the title of the header bar
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), title);

    // Create a horizontal box to hold the icon and title
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    // Load the icon from the specified path
    if (icon_path[0] != '\0')
    {

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(icon_path, NULL);
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 32, 32, GDK_INTERP_BILINEAR);
        GtkWidget *icon = gtk_image_new_from_pixbuf(scaled_pixbuf);
        // Add the icon to the box
        gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    }

    // Add the box to the header bar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), box);

    // Set the header bar as the title bar of the window
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
}

// TODO: Should be not manipulate the end of tag ">" in the file
// TODO: Should manipulate spaces and tabs and new lines
// ########################## "This function should stop reading after the greater than symbol " > " exactelly"
gchar *read_property(FILE *index, int *status)
{
    gchar *property = NULL;
    SAFE_ALLOC(property, gchar, MAX_PROPERTY_SIZE);

    int i = 0;
    gchar c;
    while ((c = fgetc(index)) != '>')
    {
        if (c == '=')
        {
            *status = 1;
            property[i] = '\0';
            return property;
        }
        if (c != ' ' && c != '\n' && c != '\t')
            property[i++] = c;
    }
    if (c == '>')
        *status = 2;
    return NULL;
}

gchar *read_value(FILE *index, int *status)
{
    gchar *value = NULL;
    SAFE_ALLOC(value, gchar, MAX_VALUE_SIZE);

    int i = 0;
    gboolean reading_flag = FALSE;
    gchar c;

    while (((c = fgetc(index)) != '"' || !reading_flag) && c != EOF)
    {
        if (c != '"' && !reading_flag)
        {
            *status = -1;
            return value;
        }

        if (c == '"' && !reading_flag)
            reading_flag = !reading_flag;

        else if (c != '\n')
            value[i++] = c;
        else
        {
            // Debugging part
            if (c == '\n')
            {
                value[i] = '\0';
                g_critical("ERROR: while reading the value %s\n", value);
                exit(EXIT_FAILURE);
            }
        }
    }
    value[i] = '\0';
    *status = 1;
    return value;
}

gboolean is_character(gchar c)
{
    return (c > 'A' && c < 'Z') || (c > 'a' && c < 'z');
}

ViewConfig *init_generic_config(FILE *index, void *config, ConfigurePropertyCallback configure_property_callback)
{
    if (!config || !index)
        return NULL;

    ViewConfig *view_config = NULL;
    SAFE_ALLOC(view_config, ViewConfig, 1);
    DFEAULT_VIEW_CONFIG(view_config);

    gchar *property = NULL;
    gchar *value = NULL;

    gchar c;
    while ((c = fgetc(index)) != '>')
    {
        if (is_character(c))
            fseek(index, -1, SEEK_CUR);

        int status = -1;
        property = read_property(index, &status);

        if (status == 2)
            return view_config;

        if (status == 1 && property)
        {
            value = read_value(index, &status);
            if (status == 1 && value)
            {
                if (g_strcmp0(property, "id") == 0)
                {
                    strcpy(view_config->view_id, value);
                    free(property);
                }
                else
                {
                    view_config = configure_property_callback(config, view_config, property, value);
                    free(value);
                    free(property);
                }
            }
        }
    }

    return view_config;
}

View *find_view_by_id(char *view_id, View *root_view)
{
    // g_print("PASSED WIDGETS WHILE SEARCHING: %s\n", root_view->view_config->view_id);
    if (g_strcmp0(root_view->view_config->view_id, view_id) == 0)
        return (View *)root_view;

    View *view = NULL;
    if (root_view->next)
    {
        view = find_view_by_id(view_id, root_view->next);
        if (view)
            return view;
    }
    if (root_view->child)
    {
        view = find_view_by_id(view_id, root_view->child);
        if (view)
            return view;
    }
    return (View *)view;
}

// Function to force width and height using CSS for a widget
void set_widget_size(GtkWidget *widget, Dimensions dimensions)
{
    gtk_widget_set_size_request(widget, dimensions.width, dimensions.height);
    // // Create a CSS provider
    // GtkCssProvider *css_provider = gtk_css_provider_new();
    // GdkScreen *screen = gtk_widget_get_screen(widget);

    // // Create the CSS rule as a string
    // gchar *css = g_strdup_printf(
    //     ".fixed-size { "
    //     "  min-width: %dpx; "
    //     "  max-width: %dpx; "
    //     "  width: %dpx; "
    //     "  min-height: %dpx; "
    //     "  max-height: %dpx; "
    //     "  height: %dpx; "
    //     "}",
    //     dimensions.width, dimensions.width, dimensions.width, dimensions.height, dimensions.height, dimensions.height);

    // // Load the CSS data into the provider
    // gtk_css_provider_load_from_data(css_provider, css, -1, NULL);

    // // Apply the CSS provider to the widget's style context
    // gtk_style_context_add_provider(
    //     gtk_widget_get_style_context(widget),
    //     GTK_STYLE_PROVIDER(css_provider),
    //     GTK_STYLE_PROVIDER_PRIORITY_USER
    // );

    // // Add the 'fixed-size' class to the widget
    // gtk_style_context_add_class(gtk_widget_get_style_context(widget), "fixed-size");

    // // Clean up
    // g_free(css);
    // g_object_unref(css_provider);
}

// Readers
ViewConfig *read_view_config_from_dialog(gboolean update_mode)
{
    static int wid_index = 1;

    // View config
    ViewConfig *view_config = NULL;
    SAFE_ALLOC(view_config, ViewConfig, 1);
    DFEAULT_VIEW_CONFIG(view_config);

    const gchar *view_id = read_config_value_as_string("view_id_entry");
    if (!update_mode)
        strcpy(view_config->view_id, g_strconcat("wid-", g_strdup_printf("%d", wid_index++), "-", view_config->view_id, view_id, NULL));
    else
        strcpy(view_config->view_id, view_id);

    // Fixed config
    gint position = read_config_value_as_int("position_x_spin");
    view_config->position_x = position;
    position = read_config_value_as_int("position_y_spin");
    view_config->position_y = position;

    // Box config
    gboolean box_expand = read_config_value_as_boolean("box_expand_switch");
    view_config->box_expand = box_expand;

    gboolean box_fill = read_config_value_as_boolean("box_fill_switch");
    view_config->box_fill = box_fill;

    gint box_padding = read_config_value_as_int("box_padding_spin");
    view_config->box_padding = box_padding;

    const gchar *pack_direction = read_config_value_as_string("pack_direction_combo");
    if (strcasecmp(pack_direction, "end") == 0)
        view_config->pack_direction = 0;
    else
        view_config->pack_direction = 1;

    // FlowBox container
    gint flow_box_order = read_config_value_as_int("flowbox_order_spin");
    view_config->flow_box_order = flow_box_order;

    // Paned config
    const gchar *paned_order = read_config_value_as_string("paned_order_combo");
    if (strcasecmp(paned_order, "First") == 0)
        view_config->paned_order = 1;
    else
        view_config->paned_order = 2;

    // Notebook config
    const gchar *tab_label = read_config_value_as_string("tab_label_entry");
    g_strlcpy(view_config->tab_label, tab_label, MAX_LABEL_SIZE);
    view_config->is_reorderable = read_config_value_as_boolean("is_reorderable_switch");

    // Grid config
    view_config->row = read_config_value_as_int("row_spin");
    view_config->column = read_config_value_as_int("column_spin");

    gint row_span = read_config_value_as_int("row_span_spin");
    view_config->row_span = row_span == 0 ? 1 : row_span;
    gint column_span = read_config_value_as_int("column_span_spin");
    view_config->column_span = column_span == 0 ? 1 : column_span;

    // Menu config
    const gchar *menu_orientation_value = read_config_value_as_string("menu_orientation_combo");
    if (menu_orientation_value)
        g_strlcpy(view_config->menu_orientation, menu_orientation_value, MAX_LABEL_SIZE);

    gint menu_top = read_config_value_as_int("menu_top_spin");
    gint menu_bottom = read_config_value_as_int("menu_bottom_spin");
    gint menu_left = read_config_value_as_int("menu_left_spin");
    gint menu_right = read_config_value_as_int("menu_right_spin");

    view_config->menu_top = menu_top;
    view_config->menu_bottom = menu_bottom == 0 ? 1 : menu_bottom;
    view_config->menu_left = menu_left;
    view_config->menu_right = menu_right == 0 ? 1 : menu_right;

    g_print("ML: %d, MR: %d, MT: %d, MB: %d\n", view_config->menu_left, view_config->menu_right, view_config->menu_top, view_config->menu_bottom);

    // Signals config
    // OnClick
    const gchar *sig_on_click_handler = read_config_value_as_string("on_click_entry");
    // if (sig_on_click_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_CLICK;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_click_handler, MAX_SIGNAL_NAME_SIZE);
    //     g_print("SIG///=> %s\n", sig_on_click_handler);
    // }

    // const gchar *sig_on_toggle_handler = read_config_value_as_string("on_toggle_entry");
    // if (sig_on_toggle_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_TOGGLE;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_toggle_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_change_handler = read_config_value_as_string("on_change_entry");
    // if (sig_on_change_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_CHANGE;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_change_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_activate_handler = read_config_value_as_string("on_activate_entry");
    // if (sig_on_activate_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_ACTIVATE;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_activate_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_delete_event_handler = read_config_value_as_string("on_delete_event_entry");
    // if (sig_on_delete_event_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_DELETE_EVENT;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_delete_event_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_destroy_handler = read_config_value_as_string("on_destroy_entry");
    // if (sig_on_destroy_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_DESTROY;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_destroy_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_value_changed_handler = read_config_value_as_string("on_value_changed_entry");
    // if (sig_on_value_changed_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_VALUE_CHANGED;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_value_changed_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_row_activated_handler = read_config_value_as_string("on_row_activated_entry");
    // if (sig_on_row_activated_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_ROW_ACTIVATED;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_row_activated_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_key_press_handler = read_config_value_as_string("on_key_press_entry");
    // if (sig_on_key_press_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_KEY_PRESS;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_key_press_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_button_press_handler = read_config_value_as_string("on_button_press_entry");
    // if (sig_on_button_press_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_BUTTON_PRESS;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_button_press_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_button_release_handler = read_config_value_as_string("on_button_release_entry");
    // if (sig_on_button_release_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_BUTTON_RELEASE;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_button_release_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_focus_in_handler = read_config_value_as_string("on_focus_in_entry");
    // if (sig_on_focus_in_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_FOCUS_IN;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_focus_in_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_focus_out_handler = read_config_value_as_string("on_focus_out_entry");
    // if (sig_on_focus_out_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_FOCUS_OUT;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_focus_out_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_selection_changed_handler = read_config_value_as_string("on_selection_changed_entry");
    // if (sig_on_selection_changed_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_SELECTION_CHANGED;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_selection_changed_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_popup_menu_handler = read_config_value_as_string("on_popup_menu_entry");
    // if (sig_on_popup_menu_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_POPUP_MENU;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_popup_menu_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_response_handler = read_config_value_as_string("on_response_entry");
    // if (sig_on_response_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_RESPONSE;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_response_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    // const gchar *sig_on_color_set_handler = read_config_value_as_string("on_color_set_entry");
    // if (sig_on_color_set_handler)
    // {
    //     view_config->signal.event_type = SIG_ON_COLOR_SET;
    //     g_strlcpy(view_config->signal.sig_handler, sig_on_color_set_handler, MAX_SIGNAL_NAME_SIZE);
    // }

    const gchar *sig_type = read_config_value_as_string("sig_type_combo");
    if (sig_type)
    {
        if (g_strcmp0(sig_type, "OnClick") == 0)
            view_config->signal.event_type = SIG_ON_CLICK;
        else if (g_strcmp0(sig_type, "OnToggle") == 0)
            view_config->signal.event_type = SIG_ON_TOGGLE;
        else if (g_strcmp0(sig_type, "OnChange") == 0)
            view_config->signal.event_type = SIG_ON_CHANGE;
        else if (g_strcmp0(sig_type, "OnActivate") == 0)
            view_config->signal.event_type = SIG_ON_ACTIVATE;
        else if (g_strcmp0(sig_type, "OnDeleteEvent") == 0)
            view_config->signal.event_type = SIG_ON_DELETE_EVENT;
        else if (g_strcmp0(sig_type, "OnDestroy") == 0)
            view_config->signal.event_type = SIG_ON_DESTROY;
        else if (g_strcmp0(sig_type, "OnValueChanged") == 0)
            view_config->signal.event_type = SIG_ON_VALUE_CHANGED;
        else if (g_strcmp0(sig_type, "OnRowActivated") == 0)
            view_config->signal.event_type = SIG_ON_ROW_ACTIVATED;
        else if (g_strcmp0(sig_type, "OnKeyPress") == 0)
            view_config->signal.event_type = SIG_ON_KEY_PRESS;
        else if (g_strcmp0(sig_type, "OnButtonPress") == 0)
            view_config->signal.event_type = SIG_ON_BUTTON_PRESS;
        else if (g_strcmp0(sig_type, "OnButtonRelease") == 0)
            view_config->signal.event_type = SIG_ON_BUTTON_RELEASE;
        else if (g_strcmp0(sig_type, "OnFocusIn") == 0)
            view_config->signal.event_type = SIG_ON_FOCUS_IN;
        else if (g_strcmp0(sig_type, "OnFocusOut") == 0)
            view_config->signal.event_type = SIG_ON_FOCUS_OUT;
        else if (g_strcmp0(sig_type, "OnSelectionChanged") == 0)
            view_config->signal.event_type = SIG_ON_SELECTION_CHANGED;
        else if (g_strcmp0(sig_type, "OnPopupMenu") == 0)
            view_config->signal.event_type = SIG_ON_POPUP_MENU;
        else if (g_strcmp0(sig_type, "OnResponse") == 0)
            view_config->signal.event_type = SIG_ON_RESPONSE;
        else if (g_strcmp0(sig_type, "OnColorSet") == 0)
            view_config->signal.event_type = SIG_ON_COLOR_SET;
    }

    if (sig_on_click_handler)
        g_strlcpy(view_config->signal.sig_handler, sig_on_click_handler, MAX_SIGNAL_NAME_SIZE);

    const gchar *params = read_config_value_as_string("sig_params_entry");
    int key = 0;
    int index = 0;
    gchar temp[MAX_VALUE_SIZE];
    int cpt = 0;
    if (params)
    {
        while (params[key] != '\0' && params[key] != '\n')
        {
            if (params[key] != ';')
            {
                temp[index++] = params[key];
            }
            else
            {
                temp[index] = '\0';
                strcpy(view_config->param[cpt++], g_strdup(temp));
                g_print("PARAMS ===> %s\n", temp);
                index = 0;
            }
            key++;
        }

        if (index > 0)
        {
            temp[index] = '\0';
            strcpy(view_config->param[cpt++], g_strdup(temp));
            g_print("PARAMS ===> %s\n", temp);
        }
    }

    return view_config;
}

const gchar *read_config_value_as_string(gchar *view_id)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return NULL;
    }
    if (GTK_IS_COMBO_BOX(input_view->widget))
        return gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(input_view->widget));
    else if (GTK_IS_ENTRY(input_view->widget))
        return gtk_entry_get_text(GTK_ENTRY(input_view->widget));
    g_print("Error: => Widget type not compatible with the expected value\n");
    return NULL;
}

gint read_config_value_as_int(gchar *view_id)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return 0;
    }
    if (GTK_IS_SPIN_BUTTON(input_view->widget))
        return gtk_spin_button_get_value(GTK_SPIN_BUTTON(input_view->widget));

    g_print("Error: => Widget type not compatible with the expected value\n");
    return 0;
}

gdouble read_config_value_as_double(gchar *view_id)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return 0;
    }
    if (GTK_IS_SPIN_BUTTON(input_view->widget))
        return gtk_spin_button_get_value(GTK_SPIN_BUTTON(input_view->widget));

    g_print("Error: => Widget type not compatible with the expected value\n");
    return 0;
}

gboolean read_config_value_as_boolean(gchar *view_id)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return false;
    }
    if (GTK_IS_SWITCH(input_view->widget))
        return gtk_switch_get_active(GTK_SWITCH(input_view->widget));

    g_print("Error: => Widget type not compatible with the expected value\n");
    return false;
}

GtkAlign read_align_config(gchar *input_combo)
{
    const gchar *align_text = read_config_value_as_string(input_combo);
    if (align_text)
    {
        if (strcasecmp(align_text, "start") == 0)
            return GTK_ALIGN_START;
        else if (strcasecmp(align_text, "end") == 0)
            return GTK_ALIGN_END;
        else if (strcasecmp(align_text, "baseline") == 0)
            return GTK_ALIGN_BASELINE;
        else if (strcasecmp(align_text, "center") == 0)
            return GTK_ALIGN_CENTER;
        else
            return GTK_ALIGN_FILL;
    }
}

GtkPositionType read_position_config(gchar *input_combo, GtkPositionType default_position)
{
    const gchar *icon_position = read_config_value_as_string(input_combo);
    if (strcasecmp(icon_position, "top") == 0)
        return GTK_POS_TOP;
    else if (strcasecmp(icon_position, "bottom") == 0)
        return GTK_POS_BOTTOM;
    else if (strcasecmp(icon_position, "left") == 0)
        return GTK_POS_LEFT;
    else if (strcasecmp(icon_position, "right") == 0)
        return GTK_POS_RIGHT;
    else
        return default_position;
}

Dimensions *read_dimensions_config()
{
    Dimensions *dimensions;
    SAFE_ALLOC(dimensions, Dimensions, 1);
    gint width = read_config_value_as_int("width_spin");
    gint height = read_config_value_as_int("height_spin");
    dimensions->width = width;
    dimensions->height = height;

    return dimensions;
}

Margins *read_margins_config()
{
    Margins *margins;
    SAFE_ALLOC(margins, Margins, 1);

    // Margin top
    gint margin_top = read_config_value_as_int("margin_top_spin");
    margins->top = margin_top;

    // Margin bottom
    gint margin_bottom = read_config_value_as_int("margin_bottom_spin");
    margins->bottom = margin_bottom;

    // Margin left
    gint margin_left = read_config_value_as_int("margin_left_spin");
    margins->start = margin_left;

    // Margin right
    gint margin_right = read_config_value_as_int("margin_right_spin");
    margins->end = margin_right;

    return margins;
}

gchar *read_text_color_from_widget(GtkWidget *widget)
{

    return g_object_get_data(G_OBJECT(widget), "text_color");

    // This approach also works
    // GtkStyleContext *context = gtk_widget_get_style_context(widget);
    // GdkRGBA fg_rgba;

    // // Get the widget's foreground color (for the normal state)
    // gtk_style_context_get_color(context, GTK_STATE_FLAG_NORMAL, &fg_rgba);
    // guint fg_r = (guint)(fg_rgba.red * 255);
    // guint fg_g = (guint)(fg_rgba.green * 255);
    // guint fg_b = (guint)(fg_rgba.blue * 255);
    // gchar *fg_hex = NULL; // Format: "#RRGGBB"
    // SAFE_ALLOC(fg_hex, gchar, 8);

    // sprintf(fg_hex, "#%02X%02X%02X", fg_r, fg_g, fg_b);

    // return fg_hex;
}

gchar *read_bg_color_from_widget(GtkWidget *widget)
{

    return g_object_get_data(G_OBJECT(widget), "bg_color");

    // This approach also works
    // if (!widget)
    //     return NULL;

    // GtkStyleContext *context = gtk_widget_get_style_context(widget);
    // GValue bg_value = G_VALUE_INIT;

    // // Get background color property
    // gtk_style_context_get_property(context, "background-color", GTK_STATE_FLAG_NORMAL, &bg_value);

    // // Check if GValue contains a GdkRGBA
    // if (G_VALUE_HOLDS(&bg_value, GDK_TYPE_RGBA))
    // {
    //     // Get the GdkRGBA from the GValue
    //     const GdkRGBA *bg_rgba = g_value_get_boxed(&bg_value);

    //     // Convert color to hex
    //     gchar *bg_hex = g_strdup_printf("#%02X%02X%02X",
    //                                     (guint)(bg_rgba->red * 255),
    //                                     (guint)(bg_rgba->green * 255),
    //                                     (guint)(bg_rgba->blue * 255));

    //     g_value_unset(&bg_value); // Free GValue memory
    //     return bg_hex;
    // }

    // g_value_unset(&bg_value); // Free GValue memory
    // return NULL;              // Return NULL if no color is found
}

gchar *read_bg_image_from_widget(GtkWidget *widget)
{
    return g_object_get_data(G_OBJECT(widget), "bg_image");
}

// Writers

void write_view_config_to_dialog(ViewConfig *view_config)
{
    if (!view_config)
        return;

    // View config
    write_config_value_as_string("view_id_entry", view_config->view_id);

    // Fixed container
    write_config_value_as_int("position_x_spin", view_config->position_x);
    write_config_value_as_int("position_y_spin", view_config->position_y);

    // Box container
    write_config_value_as_combo_index("pack_direction_combo", view_config->pack_direction == 0 ? 1 : 0);
    write_config_value_as_boolean("box_expand_switch", view_config->box_expand);
    write_config_value_as_boolean("box_fill_switch", view_config->box_fill);
    write_config_value_as_int("box_padding_spin", view_config->box_padding);

    // FlowBox container
    write_config_value_as_int("flowbox_order_spin", view_config->flow_box_order);

    // Paned container
    write_config_value_as_combo_index("paned_order_combo", view_config->paned_order == 1 ? 0 : 1);

    // NoteBook container properties
    write_config_value_as_string("tab_label_entry", view_config->tab_label);
    write_config_value_as_boolean("is_reorderable_switch", view_config->is_reorderable);

    // Grid container properties
    write_config_value_as_int("row_spin", view_config->row);
    write_config_value_as_int("column_spin", view_config->column);
    write_config_value_as_int("row_span_spin", view_config->row_span);
    write_config_value_as_int("column_span_spin", view_config->column_span);

    // Menu properties
    write_config_value_as_combo_index("menu_orientation_combo", strcasecmp(view_config->menu_orientation, "vertical") == 0 ? 0 : 1);
    write_config_value_as_int("menu_top_spin", view_config->menu_top);
    write_config_value_as_int("menu_bottom_spin", view_config->menu_bottom);
    write_config_value_as_int("menu_left_spin", view_config->menu_left);
    write_config_value_as_int("menu_right_spin", view_config->menu_right);

    // Signal properties
    write_config_value_as_string("sig_type_combo", view_config->signal.sig_handler);
    gchar params[200];
    params[0] = '\0';
    if (view_config->param)
        g_strconcat(view_config->param[0] ? view_config->param[0] : "\0", ";", view_config->param[1] ? view_config->param[1] : "\0", ";", view_config->param[2] ? view_config->param[2] : "\0", ";", view_config->param[3] ? view_config->param[3] : "\0", NULL);
    write_config_value_as_string("sig_params_entry", params);
    // Signals config
    // OnClick
    write_config_value_as_string("on_click_entry", view_config->signal.sig_handler);
}

void write_config_value_as_combo_index(gchar *view_id, int index)
{
    View *output_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!output_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return;
    }
    if (GTK_IS_COMBO_BOX_TEXT(output_view->widget))
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(output_view->widget), index);
    }
}

void write_config_value_as_string(gchar *view_id, const gchar *value)
{
    View *output_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!output_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return;
    }
    if (!value)
    {
        g_print("Error: ==> The value is null\n");
        return;
    }
    g_print("OWIDGET FOUND IS: %s\n", output_view->view_config->view_id);
    if (GTK_IS_COMBO_BOX_TEXT(output_view->widget))
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(output_view->widget), value);
    else if (GTK_IS_ENTRY(output_view->widget))
        gtk_entry_set_text(GTK_ENTRY(output_view->widget), value);
    else
        g_print("Error: => OWidget type not compatible with the expected value\n");
}

void write_config_value_as_int(gchar *view_id, gint value)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return;
    }
    if (GTK_IS_SPIN_BUTTON(input_view->widget))
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(input_view->widget), value);
    else
        g_print("Error: => Widget type not compatible with the expected value\n");
}

void write_config_value_as_double(gchar *view_id, gdouble value)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return;
    }
    if (GTK_IS_SPIN_BUTTON(input_view->widget))
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(input_view->widget), value);
    else
        g_print("Error: => Widget type not compatible with the expected value\n");
}

void write_config_value_as_boolean(gchar *view_id, gboolean value)
{
    View *input_view = find_view_by_id(view_id, root_dialog_view_global);
    if (!input_view)
    {
        g_print("Error: ==> Cannot find the %s\n", view_id);
        return;
    }
    if (GTK_IS_SWITCH(input_view->widget))
        gtk_switch_set_active(GTK_SWITCH(input_view->widget), value);
    else
        g_print("Error: => Widget type not compatible with the expected value\n");
}

void write_align_config(GtkAlign halign, GtkAlign valign)
{
    // VAlign
    gint index = 0;
    switch (halign)
    {
    case GTK_ALIGN_FILL:
        index = 0;
        break;
    case GTK_ALIGN_CENTER:
        index = 1;
        break;
    case GTK_ALIGN_START:
        index = 2;
        break;
    case GTK_ALIGN_END:
        index = 3;
        break;
    case GTK_ALIGN_BASELINE:
        index = 4;
        break;
    default:
        index = 0;
        break;
    }
    write_config_value_as_combo_index("halign_combo", index);

    switch (valign)
    {
    case GTK_ALIGN_FILL:
        index = 0;
        break;
    case GTK_ALIGN_CENTER:
        index = 1;
        break;
    case GTK_ALIGN_START:
        index = 2;
        break;
    case GTK_ALIGN_END:
        index = 3;
        break;
    case GTK_ALIGN_BASELINE:
        index = 4;
        break;
    default:
        index = 0;
        break;
    }

    write_config_value_as_combo_index("valign_combo", index);
}

void write_position_config(gchar *output_combo, GtkPositionType position)
{
    gint index = 0;
    switch (position)
    {
    case GTK_BASELINE_POSITION_CENTER:
        index = 0;
        break;
    case GTK_BASELINE_POSITION_TOP:
        index = 1;
        break;
    case GTK_BASELINE_POSITION_BOTTOM:
        index = 2;
        break;
    default:
        index = 0;
        break;
    }
    write_config_value_as_combo_index(output_combo, index);
}

void write_dimensions_config(Dimensions dimensions)
{
    // Width
    write_config_value_as_int("width_spin", dimensions.width);

    // Height
    write_config_value_as_int("height_spin", dimensions.height);
}

void write_margins_config(Margins margins)
{
    // Margins
    write_config_value_as_int("margin_top_spin", margins.top);
    write_config_value_as_int("margin_bottom_spin", margins.bottom);
    write_config_value_as_int("margin_left_spin", margins.start);
    write_config_value_as_int("margin_right_spin", margins.end);
}

void write_expand_config(gboolean hexpand, gboolean vexpand)
{
    // HExpand
    write_config_value_as_boolean("hexpand_switch", hexpand);

    // VExpand
    write_config_value_as_boolean("vexpand_switch", vexpand);
}

void write_orientation_config(gchar *output_combo, GtkOrientation orientation)
{
    write_config_value_as_combo_index(output_combo, orientation == GTK_ORIENTATION_VERTICAL ? 0 : 1);
}

// Testing
void print_graph_to_debug(View *root)
{
    if (!root)
        return;

    if (root->parent)
        g_print("WIDGET: ===> %s => PARENT ==> %s\n", root->view_config->view_id, root->parent->view_config->view_id);
    else
        g_print("WIDGET: ===> %s => PARENT ==> ROOT\n", root->view_config->view_id);
    if (root->next)
        g_print("Has next: %s\n", root->next->view_config->view_id);
    if (root->child)
        g_print("Has child: %s\n", root->child->view_config->view_id);
    print_graph_to_debug(root->child);
    print_graph_to_debug(root->next);
}
