#include "../../include/widgets/separator.h"

ViewConfig *configure_separator_property(SeparatorConfig *separator_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!separator_config || !property || !value)
        return NULL;

    // Orientation
    if (g_strcmp0(property, "orientation") == 0)
    {
        if (g_strcmp0(value, "horizontal") == 0)
            separator_config->orientation = GTK_ORIENTATION_HORIZONTAL;
        else if (g_strcmp0(value, "vertical") == 0)
            separator_config->orientation = GTK_ORIENTATION_VERTICAL;
    }

    // Visibility
    if (g_strcmp0(property, "visible") == 0)
    {
        if (g_strcmp0(value, "true") == 0)
            separator_config->is_visible = TRUE;
        else if (g_strcmp0(value, "false") == 0)
            separator_config->is_visible = FALSE;
    }

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        separator_config->margins.top = atoi(value);

    if (g_strcmp0(property, "margin_bottom") == 0)
        separator_config->margins.bottom = atoi(value);

    if (g_strcmp0(property, "margin_left") == 0)
        separator_config->margins.start = atoi(value);

    if (g_strcmp0(property, "margin_right") == 0)
        separator_config->margins.end = atoi(value);

    // Dimensions
    if (g_strcmp0(property, "width") == 0)
        separator_config->dimensions.width = atoi(value);

    if (g_strcmp0(property, "height") == 0)
        separator_config->dimensions.height = atoi(value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            separator_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            separator_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            separator_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            separator_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            separator_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            separator_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            separator_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            separator_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_separator_config(FILE *index, SeparatorConfig *separator_config)
{
    return init_generic_config(index, (void *)separator_config, (ConfigurePropertyCallback)configure_separator_property);
}

GtkWidget *create_separator(SeparatorConfig separator_config)
{

    GtkWidget *separator = NULL;

    // Creation
    separator = gtk_separator_new(separator_config.orientation);

    // Set visibility
    if (separator_config.is_visible)
        gtk_widget_show(separator);
    else
        gtk_widget_hide(separator);

    if (separator_config.dimensions.width > 0 || separator_config.dimensions.height > 0)
        gtk_widget_set_size_request(separator, separator_config.dimensions.width, separator_config.dimensions.height);

    widget_set_margins(separator, separator_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(separator, separator_config.hexpand);
    gtk_widget_set_vexpand(separator, separator_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(separator, separator_config.halign);
    gtk_widget_set_valign(separator, separator_config.valign);

    return separator;
}

SeparatorConfig *read_separator_config_from_dialog()
{
    SeparatorConfig *separator_config_ptr = NULL;
    SAFE_ALLOC(separator_config_ptr, SeparatorConfig, 1);

    SeparatorConfig separator_config = DEFAULT_SEPARATOR;

    // Tooltip
    const gchar *tooltip = read_config_value_as_string("tooltip_entry");
    strcpy(separator_config.tooltip, tooltip);

    // Orientation
    const gchar *orientation = read_config_value_as_string("orientation_combo");
    if (strcasecmp(orientation, "horizontal") == 0)
        separator_config.orientation = GTK_ORIENTATION_HORIZONTAL;
    else
        separator_config.orientation = GTK_ORIENTATION_VERTICAL;

    // Visibility
    gboolean is_visible = read_config_value_as_boolean("visible_switch");
    separator_config.is_visible = is_visible;

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    separator_config.dimensions.width = dimensions->width;
    separator_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    separator_config.margins.top = margins->top;
    separator_config.margins.bottom = margins->bottom;
    separator_config.margins.start = margins->start;
    separator_config.margins.end = margins->end;

    // HAlign
    separator_config.halign = read_align_config("halign_combo");

    // VAlign
    separator_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    separator_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    separator_config.vexpand = vexpand;

    memcpy(separator_config_ptr, &separator_config, sizeof(SeparatorConfig));
    return separator_config_ptr;
}

SeparatorConfig *read_separator_config_from_widget(GtkWidget *widget)
{
    SeparatorConfig *separator_config_ptr = NULL;
    SAFE_ALLOC(separator_config_ptr, SeparatorConfig, 1);

    SeparatorConfig separator_config = DEFAULT_SEPARATOR;

    // Tooltip
    const gchar *tooltip = gtk_widget_get_tooltip_text(widget);
    if (tooltip)
        strcpy(separator_config.tooltip, tooltip);

    // Orientation
    GtkOrientation orientation = gtk_orientable_get_orientation(GTK_ORIENTABLE(widget));
    separator_config.orientation = orientation;

    // Visibility
    gboolean is_visible = gtk_widget_get_visible(widget);
    separator_config.is_visible = is_visible;

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    separator_config.dimensions.width = allocation.width;
    separator_config.dimensions.height = allocation.height;

    // Expand
    separator_config.hexpand = gtk_widget_get_hexpand(widget);
    separator_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    separator_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    separator_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    separator_config.margins = margins;

    memcpy(separator_config_ptr, &separator_config, sizeof(SeparatorConfig));

    return separator_config_ptr;
}

gchar *write_separator_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "separator", tabs_number);

    // Get the GtkSeparator from the view
    GtkSeparator *separator = GTK_SEPARATOR(view->widget);

    // Get the tooltip text
    const gchar *tooltip = gtk_widget_get_tooltip_text(GTK_WIDGET(separator));
    if (g_strcmp0(tooltip, "\0") != 0) // Check if the tooltip text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "tooltip=\"%s\"\n", tooltip);
    }

    // Get the orientation
    GtkOrientation orientation = gtk_orientable_get_orientation(GTK_ORIENTABLE(separator));
    if (orientation != GTK_ORIENTATION_VERTICAL) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "orientation=\"%s\"\n", orientation == GTK_ORIENTATION_HORIZONTAL ? "horizontal" : "vertical");
    }

    // Check if the separator is visible
    gboolean is_visible = gtk_widget_get_visible(GTK_WIDGET(separator));
    if (is_visible != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_visible=\"%s\"\n", is_visible ? "true" : "false");
    }

    // Get the horizontal expand property
    gboolean hexpand = gtk_widget_get_hexpand(GTK_WIDGET(separator));
    if (hexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "hexpand=\"%s\"\n", hexpand ? "true" : "false");
    }

    // Get the vertical expand property
    gboolean vexpand = gtk_widget_get_vexpand(GTK_WIDGET(separator));
    if (vexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "vexpand=\"%s\"\n", vexpand ? "true" : "false");
    }

    return "separator";
}
