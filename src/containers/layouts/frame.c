#include "./../../../include/containers/layouts/frame.h"

ViewConfig *configure_frame_property(FrameConfig *frame_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!frame_config || !property || !value)
        return NULL;

    // Set frame label
    if (g_strcmp0(property, "label") == 0)
        strcpy(frame_config->label, value);

    // Set label alignment (0.0 to 1.0)
    else if (g_strcmp0(property, "label_yalign") == 0)
        frame_config->label_yalign = atof(value);

    // Set label horizontal alignment
    else if (g_strcmp0(property, "label_xalign") == 0)
        frame_config->label_xalign = atof(value);

    // Set shadow type
    else if (g_strcmp0(property, "shadow_type") == 0)
    {
        if (g_strcmp0(value, "none") == 0)
            frame_config->shadow_type = GTK_SHADOW_NONE;
        else if (g_strcmp0(value, "in") == 0)
            frame_config->shadow_type = GTK_SHADOW_IN;
        else if (g_strcmp0(value, "out") == 0)
            frame_config->shadow_type = GTK_SHADOW_OUT;
        else if (g_strcmp0(value, "etched_in") == 0)
            frame_config->shadow_type = GTK_SHADOW_ETCHED_IN;
        else if (g_strcmp0(value, "etched_out") == 0)
            frame_config->shadow_type = GTK_SHADOW_ETCHED_OUT;
    }

    // Set background color
    else if (g_strcmp0(property, "bg_color") == 0)
        strcpy(frame_config->bg_color, value);

    // Set text color
    else if (g_strcmp0(property, "text_color") == 0)
        strcpy(frame_config->text_color, value);

    // Set widget dimensions
    else if (g_strcmp0(property, "width") == 0)
        frame_config->dimensions.width = atoi(value);

    else if (g_strcmp0(property, "height") == 0)
        frame_config->dimensions.height = atoi(value);

    // Set margins
    else if (g_strcmp0(property, "margin_top") == 0)
        frame_config->margins.top = atoi(value);

    else if (g_strcmp0(property, "margin_bottom") == 0)
        frame_config->margins.bottom = atoi(value);

    else if (g_strcmp0(property, "margin_left") == 0)
        frame_config->margins.start = atoi(value);

    else if (g_strcmp0(property, "margin_right") == 0)
        frame_config->margins.end = atoi(value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            frame_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            frame_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            frame_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            frame_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            frame_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            frame_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            frame_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            frame_config->halign = GTK_ALIGN_FILL;
    }

    // Store additional properties in view_config (if needed)
    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_frame_config(FILE *index, FrameConfig *frame_config)
{
    return init_generic_config(index, (void *)frame_config, (ConfigurePropertyCallback)configure_frame_property);
}

GtkWidget *create_frame(FrameConfig frame_config)
{
    // Create a new GtkFrame widget
    GtkWidget *frame = gtk_frame_new(frame_config.label);

    // Set label alignment
    if (frame_config.label_xalign > 0.0 && frame_config.label_yalign > 0.0)
        gtk_frame_set_label_align(GTK_FRAME(frame), frame_config.label_xalign, frame_config.label_yalign);
    else
        gtk_frame_set_label(GTK_FRAME(frame), frame_config.label);
    // Set shadow type
    gtk_frame_set_shadow_type(GTK_FRAME(frame), frame_config.shadow_type);

    // Set widget size if specified
    if (frame_config.dimensions.width > 0 && frame_config.dimensions.height > 0)
        set_widget_size(frame, frame_config.dimensions);

    // Apply background and text colors if provided
    if (strcmp(frame_config.bg_color, "\0") != 0 && strcmp(frame_config.text_color, "\0") != 0)
        widget_set_colors(frame, frame_config.bg_color, frame_config.text_color);

    // Set margins
    widget_set_margins(frame, frame_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(frame, frame_config.hexpand);
    gtk_widget_set_vexpand(frame, frame_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(frame, frame_config.halign);
    gtk_widget_set_valign(frame, frame_config.valign);

    // gtk_widget_set_halign(frame, GTK_ALIGN_START);
    // gtk_widget_set_valign(frame, GTK_ALIGN_START);

    // gtk_widget_set_hexpand(frame, TRUE);
    // gtk_widget_set_vexpand(frame, TRUE);

    return frame;
}

void frame_add_child(GtkWidget *frame, GtkWidget *child)
{
    if (!frame || !child)
        return;

    // Add the child widget to the frame
    gtk_container_add(GTK_CONTAINER(frame), child);
}

FrameConfig *read_frame_config_from_dialog()
{
    FrameConfig *frame_config_ptr = NULL;
    SAFE_ALLOC(frame_config_ptr, FrameConfig, 1);

    FrameConfig frame_config = DEFAULT_FRAME;

    // Label
    const gchar *label = read_config_value_as_string("label_entry");
    strcpy(frame_config.label, label);

    // Shadow type
    const gchar *shadow_type = read_config_value_as_string("shadow_type_combo");
    if (strcasecmp(shadow_type, "none") == 0)
        frame_config.shadow_type = GTK_SHADOW_NONE;
    else if (strcasecmp(shadow_type, "out") == 0)
        frame_config.shadow_type = GTK_SHADOW_OUT;
    else if (strcasecmp(shadow_type, "etched in") == 0)
        frame_config.shadow_type = GTK_SHADOW_ETCHED_IN;
    else if (strcasecmp(shadow_type, "etched out") == 0)
        frame_config.shadow_type = GTK_SHADOW_ETCHED_OUT;
    else
        frame_config.shadow_type = GTK_SHADOW_IN;

    // Label alignment
    frame_config.label_xalign = (gfloat)read_config_value_as_double("label_xalign_spin");
    frame_config.label_yalign = (gfloat)read_config_value_as_double("label_yalign_spin");

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    frame_config.dimensions.width = dimensions->width;
    frame_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    frame_config.margins.top = margins->top;
    frame_config.margins.bottom = margins->bottom;
    frame_config.margins.start = margins->start;
    frame_config.margins.end = margins->end;

    // HAlign
    frame_config.halign = read_align_config("halign_combo");

    // VAlign
    frame_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    frame_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    frame_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(frame_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(frame_config.text_color, text_color);

    memcpy(frame_config_ptr, &frame_config, sizeof(FrameConfig));
    return frame_config_ptr;
}

FrameConfig *read_frame_config_from_widget(GtkWidget *widget)
{
    FrameConfig *frame_config_ptr = NULL;
    SAFE_ALLOC(frame_config_ptr, FrameConfig, 1);

    FrameConfig frame_config = DEFAULT_FRAME;

    // Label
    const gchar *label = gtk_frame_get_label(GTK_FRAME(widget));
    if (label)
        strcpy(frame_config.label, label);

    // Shadow type
    GtkShadowType shadow_type = gtk_frame_get_shadow_type(GTK_FRAME(widget));
    frame_config.shadow_type = shadow_type;

    // Label alignment
    gfloat label_xalign, label_yalign;
    gtk_frame_get_label_align(GTK_FRAME(widget), &label_xalign, &label_yalign);
    frame_config.label_xalign = label_xalign;
    frame_config.label_yalign = label_yalign;

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    frame_config.dimensions.width = allocation.width;
    frame_config.dimensions.height = allocation.height;

    // Expand
    frame_config.hexpand = gtk_widget_get_hexpand(widget);
    frame_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    frame_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    frame_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    frame_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(frame_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(frame_config.text_color, property_value);

    memcpy(frame_config_ptr, &frame_config, sizeof(FrameConfig));

    return frame_config_ptr;
}

gchar *write_frame_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "frame", tabs_number);

    // Get the GtkFrame from the view
    GtkFrame *frame = GTK_FRAME(view->widget);

    // Get the label text
    const gchar *label = gtk_frame_get_label(frame);
    if (g_strcmp0(label, "\0") != 0) // Check if the label text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "label=\"%s\"\n", label);
    }

    // Get the label's vertical and horizontal alignment
    gfloat label_xalign, label_yalign;
    gtk_frame_get_label_align(frame, &label_xalign, &label_yalign);

    if (label_yalign != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "label_yalign=\"%f\"\n", label_yalign);
    }

    if (label_xalign != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "label_xalign=\"%f\"\n", label_xalign);
    }

    // Get the shadow type
    GtkShadowType shadow_type = gtk_frame_get_shadow_type(frame);
    if (shadow_type != GTK_SHADOW_IN) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "shadow_type=\"%s\"\n", shadow_type == GTK_SHADOW_NONE ? "none" : shadow_type == GTK_SHADOW_IN       ? "in"
                                                                                           : shadow_type == GTK_SHADOW_OUT        ? "out"
                                                                                           : shadow_type == GTK_SHADOW_ETCHED_IN  ? "etched_in"
                                                                                           : shadow_type == GTK_SHADOW_ETCHED_OUT ? "etched_out"
                                                                                                                                  : "unknown");
    }

    // Get the horizontal expand property
    gboolean hexpand = gtk_widget_get_hexpand(GTK_WIDGET(frame));
    if (hexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "hexpand=\"%s\"\n", hexpand ? "true" : "false");
    }

    // Get the vertical expand property
    gboolean vexpand = gtk_widget_get_vexpand(GTK_WIDGET(frame));
    if (vexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "vexpand=\"%s\"\n", vexpand ? "true" : "false");
    }

    return "frame";
}
