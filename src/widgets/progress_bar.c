#include "../../include/widgets/progress_bar.h"

ViewConfig *configure_progress_bar_property(ProgressBarConfig *progress_bar_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!progress_bar_config || !property || !value)
        return NULL;

    // Progress fraction
    if (g_strcmp0(property, "progress_fraction") == 0)
        progress_bar_config->progress_fraction = atof(value);

    // Pulse step
    if (g_strcmp0(property, "progress_pulse_step") == 0)
        progress_bar_config->progress_pulse_step = atof(value);

    // Text
    if (g_strcmp0(property, "text") == 0)
        strcpy(progress_bar_config->text, value);

    // Text visibility
    if (g_strcmp0(property, "is_text_visible") == 0)
        progress_bar_config->is_text_visible = g_strcmp0(value, "true") == 0;

    // Inverted
    if (g_strcmp0(property, "is_inverted") == 0)
        progress_bar_config->is_inverted = g_strcmp0(value, "true") == 0;

    // Ellipsize
    if (g_strcmp0(property, "ellipsize") == 0)
    {
        if (g_strcmp0(value, "middle") == 0)
            progress_bar_config->ellipsize = PANGO_ELLIPSIZE_MIDDLE;
        else if (g_strcmp0(value, "start") == 0)
            progress_bar_config->ellipsize = PANGO_ELLIPSIZE_START;
        else if (g_strcmp0(value, "end") == 0)
            progress_bar_config->ellipsize = PANGO_ELLIPSIZE_END;
        else if (g_strcmp0(value, "none") == 0)
            progress_bar_config->ellipsize = PANGO_ELLIPSIZE_NONE;
    }

    // Opacity
    if (g_strcmp0(property, "opacity") == 0)
        progress_bar_config->opacity = atof(value);

    // Dimensions
    if (g_strcmp0(property, "width") == 0)
        progress_bar_config->dimensions.width = atoi(value);

    if (g_strcmp0(property, "height") == 0)
        progress_bar_config->dimensions.height = atoi(value);

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        progress_bar_config->margins.top = atoi(value);

    if (g_strcmp0(property, "margin_bottom") == 0)
        progress_bar_config->margins.bottom = atoi(value);

    if (g_strcmp0(property, "margin_left") == 0)
        progress_bar_config->margins.start = atoi(value);

    if (g_strcmp0(property, "margin_right") == 0)
        progress_bar_config->margins.end = atoi(value);

    if (g_strcmp0(property, "bg_color") == 0)
        strcpy(progress_bar_config->bg_color, value);

    if (g_strcmp0(property, "text_color") == 0)
        strcpy(progress_bar_config->text_color, value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            progress_bar_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            progress_bar_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            progress_bar_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            progress_bar_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            progress_bar_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            progress_bar_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            progress_bar_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            progress_bar_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_progress_bar_config(FILE *index, ProgressBarConfig *progress_bar_config)
{
    return init_generic_config(index, (void *)progress_bar_config, (ConfigurePropertyCallback)configure_progress_bar_property);
}

GtkWidget *create_progress_bar(ProgressBarConfig progress_bar_config)
{

    GtkWidget *progress_bar = gtk_progress_bar_new();

    if (progress_bar_config.text[0] != '\0')
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.text);

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.progress_fraction);

    if (progress_bar_config.progress_pulse_step > 0)
        gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.progress_pulse_step);

    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.is_text_visible);
    gtk_progress_bar_set_inverted(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.is_inverted);
    gtk_progress_bar_set_ellipsize(GTK_PROGRESS_BAR(progress_bar), progress_bar_config.ellipsize);
    gtk_widget_set_opacity(progress_bar, progress_bar_config.opacity);

    if (progress_bar_config.dimensions.width > 0 && progress_bar_config.dimensions.height > 0)
        gtk_widget_set_size_request(progress_bar, progress_bar_config.dimensions.width, progress_bar_config.dimensions.height);

    widget_set_colors(progress_bar, progress_bar_config.bg_color, progress_bar_config.text_color);

    widget_set_margins(progress_bar, progress_bar_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(progress_bar, progress_bar_config.hexpand);
    gtk_widget_set_vexpand(progress_bar, progress_bar_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(progress_bar, progress_bar_config.halign);
    gtk_widget_set_valign(progress_bar, progress_bar_config.valign);

    return progress_bar;
}

ProgressBarConfig *read_progress_bar_config_from_dialog()
{
    ProgressBarConfig *progress_bar_config_ptr = NULL;
    SAFE_ALLOC(progress_bar_config_ptr, ProgressBarConfig, 1);

    ProgressBarConfig progress_bar_config = DEFAULT_PROGRESS_BAR;

    // Progress fraction
    progress_bar_config.progress_fraction = read_config_value_as_double("progress_fraction_spin");

    // Pulse step
    progress_bar_config.progress_pulse_step = read_config_value_as_double("progress_pulse_step_spin");

    // Text
    const gchar *text = read_config_value_as_string("text_entry");
    strcpy(progress_bar_config.text, text);

    // Text visibility
    gboolean is_text_visible = read_config_value_as_boolean("text_visible_switch");
    progress_bar_config.is_text_visible = is_text_visible;

    // Inverted
    gboolean is_inverted = read_config_value_as_boolean("inverted_switch");
    progress_bar_config.is_inverted = is_inverted;

    // Ellipsize
    const gchar *ellipsize = read_config_value_as_string("ellipsize_combo");
    if (strcasecmp(ellipsize, "middle") == 0)
        progress_bar_config.ellipsize = PANGO_ELLIPSIZE_MIDDLE;
    else if (strcasecmp(ellipsize, "start") == 0)
        progress_bar_config.ellipsize = PANGO_ELLIPSIZE_START;
    else if (strcasecmp(ellipsize, "none") == 0)
        progress_bar_config.ellipsize = PANGO_ELLIPSIZE_NONE;
    else
        progress_bar_config.ellipsize = PANGO_ELLIPSIZE_END;

    // // Opacity
    // progress_bar_config.opacity = read_config_value_as_double("opacity_spin");

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    progress_bar_config.dimensions.width = dimensions->width;
    progress_bar_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    progress_bar_config.margins.top = margins->top;
    progress_bar_config.margins.bottom = margins->bottom;
    progress_bar_config.margins.start = margins->start;
    progress_bar_config.margins.end = margins->end;

    // HAlign
    progress_bar_config.halign = read_align_config("halign_combo");

    // VAlign
    progress_bar_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    progress_bar_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    progress_bar_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(progress_bar_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(progress_bar_config.text_color, text_color);

    memcpy(progress_bar_config_ptr, &progress_bar_config, sizeof(ProgressBarConfig));
    return progress_bar_config_ptr;
}

ProgressBarConfig *read_progress_bar_config_from_widget(GtkWidget *widget)
{
    ProgressBarConfig *progress_bar_config_ptr = NULL;
    SAFE_ALLOC(progress_bar_config_ptr, ProgressBarConfig, 1);

    ProgressBarConfig progress_bar_config = DEFAULT_PROGRESS_BAR;

    // Progress fraction
    progress_bar_config.progress_fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(widget));

    // Pulse step
    progress_bar_config.progress_pulse_step = gtk_progress_bar_get_pulse_step(GTK_PROGRESS_BAR(widget));

    // Text
    const gchar *text = gtk_progress_bar_get_text(GTK_PROGRESS_BAR(widget));
    if (text)
        strcpy(progress_bar_config.text, text);

    // Text visibility
    progress_bar_config.is_text_visible = gtk_progress_bar_get_show_text(GTK_PROGRESS_BAR(widget));

    // Inverted
    progress_bar_config.is_inverted = gtk_progress_bar_get_inverted(GTK_PROGRESS_BAR(widget));

    // Ellipsize
    progress_bar_config.ellipsize = gtk_progress_bar_get_ellipsize(GTK_PROGRESS_BAR(widget));

    // Opacity
    // progress_bar_config.opacity = gtk_widget_get_opacity(widget);

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    progress_bar_config.dimensions.width = allocation.width;
    progress_bar_config.dimensions.height = allocation.height;

    // Expand
    progress_bar_config.hexpand = gtk_widget_get_hexpand(widget);
    progress_bar_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    progress_bar_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    progress_bar_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    progress_bar_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(progress_bar_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(progress_bar_config.text_color, property_value);

    memcpy(progress_bar_config_ptr, &progress_bar_config, sizeof(ProgressBarConfig));

    return progress_bar_config_ptr;
}

gchar *write_progress_bar_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "progress_bar", tabs_number);

    // Get the GtkProgressBar from the view
    GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(view->widget);

    // Get the text
    const gchar *text = gtk_progress_bar_get_text(progress_bar);
    if (g_strcmp0(text, "\0") != 0) // Check if the text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "text=\"%s\"\n", text);
    }

    // Get the progress fraction
    gdouble progress_fraction = gtk_progress_bar_get_fraction(progress_bar);
    if (progress_fraction != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "progress_fraction=\"%f\"\n", progress_fraction);
    }

    // Get the progress pulse step
    gdouble progress_pulse_step = gtk_progress_bar_get_pulse_step(progress_bar);
    if (progress_pulse_step != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "progress_pulse_step=\"%f\"\n", progress_pulse_step);
    }

    // Check if the text is visible
    gboolean is_text_visible = gtk_progress_bar_get_show_text(progress_bar);
    if (is_text_visible != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_text_visible=\"%s\"\n", is_text_visible ? "true" : "false");
    }

    // Check if the progress bar is inverted
    gboolean is_inverted = gtk_progress_bar_get_inverted(progress_bar);
    if (is_inverted != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_inverted=\"%s\"\n", is_inverted ? "true" : "false");
    }

    // Get the ellipsize mode
    PangoEllipsizeMode ellipsize = gtk_progress_bar_get_ellipsize(progress_bar);
    if (ellipsize != PANGO_ELLIPSIZE_END) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "ellipsize=\"%s\"\n", ellipsize == PANGO_ELLIPSIZE_NONE ? "none" : ellipsize == PANGO_ELLIPSIZE_START ? "start"
                                                                                            : ellipsize == PANGO_ELLIPSIZE_MIDDLE  ? "middle"
                                                                                            : ellipsize == PANGO_ELLIPSIZE_END     ? "end"
                                                                                                                                   : "unknown");
    }

    // Get the opacity
    gdouble opacity = gtk_widget_get_opacity(GTK_WIDGET(progress_bar));
    if (opacity != 1.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "opacity=\"%f\"\n", opacity);
    }

    return "progress_bar";
}
