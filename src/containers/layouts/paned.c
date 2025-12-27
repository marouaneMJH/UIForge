#include "./../../../include/containers/layouts/paned.h"

ViewConfig *configure_paned_property(PanedConfig *paned_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!paned_config || !property || !value)
        return NULL;

    if (g_strcmp0(property, "orientation") == 0)
    {
        if (g_strcmp0(value, "horizontal") == 0)
            paned_config->orientation = GTK_ORIENTATION_HORIZONTAL;
        else if (g_strcmp0(value, "vertical") == 0)
            paned_config->orientation = GTK_ORIENTATION_VERTICAL;
    }

    if (g_strcmp0(property, "position") == 0)
        paned_config->position = atoi(value);

    if (g_strcmp0(property, "is_wide") == 0)
        paned_config->is_wide = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        paned_config->margins.top = atoi(value);
    if (g_strcmp0(property, "margin_bottom") == 0)
        paned_config->margins.bottom = atoi(value);
    if (g_strcmp0(property, "margin_left") == 0)
        paned_config->margins.start = atoi(value);
    if (g_strcmp0(property, "margin_right") == 0)
        paned_config->margins.end = atoi(value);

    // Colors
    if (g_strcmp0(property, "bg_color") == 0)
        strcpy(paned_config->bg_color, value);

    if (g_strcmp0(property, "text_color") == 0)
        strcpy(paned_config->text_color, value);

    // Dimensions
    if (g_strcmp0(property, "width") == 0)
        paned_config->dimensions.width = atoi(value);
    if (g_strcmp0(property, "height") == 0)
        paned_config->dimensions.height = atoi(value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            paned_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            paned_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            paned_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            paned_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            paned_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            paned_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            paned_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            paned_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_paned_config(FILE *index, PanedConfig *paned_config)
{
    return init_generic_config(index, (void *)paned_config, (ConfigurePropertyCallback)configure_paned_property);
}

GtkWidget *create_paned(PanedConfig paned_config)
{
    GtkWidget *paned = NULL;

    // Creation
    paned = gtk_paned_new(paned_config.orientation);

    // Enable or disable wide handle
    gtk_paned_set_wide_handle(GTK_PANED(paned), paned_config.is_wide);

    // Set paned position of the separator
    gtk_paned_set_position(GTK_PANED(paned), paned_config.position);

    // Set dimensions
    if (paned_config.dimensions.width > 0 || paned_config.dimensions.height)
        gtk_widget_set_size_request(paned, paned_config.dimensions.width, paned_config.dimensions.height);

    // Set colors
    widget_set_colors(paned, paned_config.bg_color, paned_config.text_color);

    // Set margins
    widget_set_margins(paned, paned_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(paned, paned_config.hexpand);
    gtk_widget_set_vexpand(paned, paned_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(paned, paned_config.halign);
    gtk_widget_set_valign(paned, paned_config.valign);

    // Set spacing
    // gtk_paned_set_spacing(GTK_PANED(paned), paned_config.spacing);
    // Set packing
    // gtk_paned_set_child_packing(GTK_PANED(paned), child_widget, is_expand, is_fill, padding, GTK_PACK_START)

    return paned;
}

PanedConfig *read_paned_config_from_dialog()
{
    PanedConfig *paned_config_ptr = NULL;
    SAFE_ALLOC(paned_config_ptr, PanedConfig, 1);

    PanedConfig paned_config = DEFAULT_PANED;

    // Orientation
    const gchar *orientation = read_config_value_as_string("orientation_combo");
    if (strcasecmp(orientation, "vertical") == 0)
        paned_config.orientation = GTK_ORIENTATION_VERTICAL;
    else
        paned_config.orientation = GTK_ORIENTATION_HORIZONTAL;

    // Position
    paned_config.position = read_config_value_as_int("position_spin");

    // Is Wide
    paned_config.is_wide = read_config_value_as_boolean("is_wide_switch");

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    paned_config.dimensions.width = dimensions->width;
    paned_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    paned_config.margins.top = margins->top;
    paned_config.margins.bottom = margins->bottom;
    paned_config.margins.start = margins->start;
    paned_config.margins.end = margins->end;

    // HAlign
    paned_config.halign = read_align_config("halign_combo");

    // VAlign
    paned_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    paned_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    paned_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(paned_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(paned_config.text_color, text_color);

    memcpy(paned_config_ptr, &paned_config, sizeof(PanedConfig));
    return paned_config_ptr;
}

PanedConfig *read_paned_config_from_widget(GtkWidget *widget)
{
    PanedConfig *paned_config_ptr = NULL;
    SAFE_ALLOC(paned_config_ptr, PanedConfig, 1);

    PanedConfig paned_config = DEFAULT_PANED;

    // Orientation
    paned_config.orientation = gtk_orientable_get_orientation(GTK_ORIENTABLE(widget));

    // Position
    paned_config.position = gtk_paned_get_position(GTK_PANED(widget));

    // Is Wide
    paned_config.is_wide = gtk_paned_get_wide_handle(GTK_PANED(widget));

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    paned_config.dimensions.width = allocation.width;
    paned_config.dimensions.height = allocation.height;

    // Expand
    paned_config.hexpand = gtk_widget_get_hexpand(widget);
    paned_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    paned_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    paned_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    paned_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(paned_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(paned_config.text_color, property_value);

    memcpy(paned_config_ptr, &paned_config, sizeof(PanedConfig));

    return paned_config_ptr;
}

gchar *write_paned_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "paned", tabs_number);

    // Get the GtkPaned from the view
    GtkPaned *paned = GTK_PANED(view->widget);

    // Get the orientation
    GtkOrientation orientation = gtk_orientable_get_orientation(GTK_ORIENTABLE(paned));
    if (orientation != GTK_ORIENTATION_VERTICAL) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "orientation=\"%s\"\n", orientation == GTK_ORIENTATION_HORIZONTAL ? "horizontal" : "vertical");
    }

    // Get the position of the separator
    gint position = gtk_paned_get_position(paned);
    if (position != 200) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "position=\"%d\"\n", position);
    }

    // Check if the paned has a wide handle
    gboolean is_wide = gtk_paned_get_wide_handle(paned);
    if (is_wide != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_wide=\"%s\"\n", is_wide ? "true" : "false");
    }

    // Get the horizontal expand property
    gboolean hexpand = gtk_widget_get_hexpand(GTK_WIDGET(paned));
    if (hexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "hexpand=\"%s\"\n", hexpand ? "true" : "false");
    }

    // Get the vertical expand property
    gboolean vexpand = gtk_widget_get_vexpand(GTK_WIDGET(paned));
    if (vexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "vexpand=\"%s\"\n", vexpand ? "true" : "false");
    }

    return "paned";
}