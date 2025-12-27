#include "./../../../include/containers/layouts/stack.h"

ViewConfig *configure_stack_property(StackConfig *stack_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!stack_config || !property || !value)
        return NULL;

    // Set whether the stack should have homogeneous children
    if (g_strcmp0(property, "is_homogeneous") == 0)
        stack_config->is_homogeneous = g_strcmp0(value, "true") == 0;

    // Enable or disable transitions
    else if (g_strcmp0(property, "is_transition_enabled") == 0)
        stack_config->is_transition_enabled = g_strcmp0(value, "true") == 0;

    // Set transition duration in milliseconds
    else if (g_strcmp0(property, "transition_duration") == 0)
        stack_config->transition_duration = atoi(value);

    // Set transition type
    else if (g_strcmp0(property, "transition_type") == 0)
    {
        if (g_strcmp0(value, "slide_left") == 0)
            stack_config->transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT;
        else if (g_strcmp0(value, "slide_right") == 0)
            stack_config->transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT;
        else if (g_strcmp0(value, "slide_up") == 0)
            stack_config->transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_UP;
        else if (g_strcmp0(value, "slide_down") == 0)
            stack_config->transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN;
        else if (g_strcmp0(value, "crossfade") == 0)
            stack_config->transition_type = GTK_STACK_TRANSITION_TYPE_CROSSFADE;
    }

    // Set background color
    else if (g_strcmp0(property, "bg_color") == 0)
        strcpy(stack_config->bg_color, value);

    // Set text color
    else if (g_strcmp0(property, "text_color") == 0)
        strcpy(stack_config->text_color, value);

    // Set widget dimensions
    else if (g_strcmp0(property, "width") == 0)
        stack_config->dimensions.width = atoi(value);

    else if (g_strcmp0(property, "height") == 0)
        stack_config->dimensions.height = atoi(value);

    // Set margins
    else if (g_strcmp0(property, "margin_top") == 0)
        stack_config->margins.top = atoi(value);

    else if (g_strcmp0(property, "margin_bottom") == 0)
        stack_config->margins.bottom = atoi(value);

    else if (g_strcmp0(property, "margin_left") == 0)
        stack_config->margins.start = atoi(value);

    else if (g_strcmp0(property, "margin_right") == 0)
        stack_config->margins.end = atoi(value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            stack_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            stack_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            stack_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            stack_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            stack_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            stack_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            stack_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            stack_config->halign = GTK_ALIGN_FILL;
    }

    // Store additional properties in view_config (if needed)
    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_stack_config(FILE *index, StackConfig *stack_config)
{
    return init_generic_config(index, (void *)stack_config, (ConfigurePropertyCallback)configure_stack_property);
}

GtkWidget *create_stack(StackConfig stack_config)
{
    // Create a new GtkStack widget
    GtkWidget *stack = gtk_stack_new();

    // Set homogeneous mode (whether all children have the same size)
    gtk_stack_set_homogeneous(GTK_STACK(stack), stack_config.is_homogeneous);

    // Enable or disable transitions
    if (stack_config.is_transition_enabled)
        gtk_stack_get_transition_running(GTK_STACK(stack));

    // Set transition duration (milliseconds)
    gtk_stack_set_transition_duration(GTK_STACK(stack), stack_config.transition_duration);

    // Set transition type (slide, crossfade, etc.)
    gtk_stack_set_transition_type(GTK_STACK(stack), stack_config.transition_type);

    // Set widget size if specified
    if (stack_config.dimensions.width > 0 && stack_config.dimensions.height > 0)
        gtk_widget_set_size_request(stack, stack_config.dimensions.width, stack_config.dimensions.height);

    // Apply background and text colors if provided
    widget_set_colors(stack, stack_config.bg_color, stack_config.text_color);

    // Set margins
    widget_set_margins(stack, stack_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(stack, stack_config.hexpand);
    gtk_widget_set_vexpand(stack, stack_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(stack, stack_config.halign);
    gtk_widget_set_valign(stack, stack_config.valign);

    return stack;
}

StackConfig *read_stack_config_from_dialog()
{
    StackConfig *stack_config_ptr = NULL;
    SAFE_ALLOC(stack_config_ptr, StackConfig, 1);

    StackConfig stack_config = DEFAULT_STACK;

    // Homogeneous
    stack_config.is_homogeneous = read_config_value_as_boolean("is_homogeneous_switch");

    // Transition enabled
    stack_config.is_transition_enabled = read_config_value_as_boolean("is_transition_enabled_switch");

    // Transition duration
    stack_config.transition_duration = read_config_value_as_int("transition_duration_spin");

    // Transition type
    const gchar *transition_type = read_config_value_as_string("transition_type_combo");
    if (strcasecmp(transition_type, "slide left") == 0)
        stack_config.transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT;
    else if (strcasecmp(transition_type, "slide right") == 0)
        stack_config.transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT;
    else if (strcasecmp(transition_type, "slide up") == 0)
        stack_config.transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_UP;
    else if (strcasecmp(transition_type, "slide down") == 0)
        stack_config.transition_type = GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN;
    else
        stack_config.transition_type = GTK_STACK_TRANSITION_TYPE_CROSSFADE;

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    stack_config.dimensions.width = dimensions->width;
    stack_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    stack_config.margins.top = margins->top;
    stack_config.margins.bottom = margins->bottom;
    stack_config.margins.start = margins->start;
    stack_config.margins.end = margins->end;

    // HAlign
    stack_config.halign = read_align_config("halign_combo");

    // VAlign
    stack_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    stack_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    stack_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(stack_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(stack_config.text_color, text_color);

    memcpy(stack_config_ptr, &stack_config, sizeof(StackConfig));
    return stack_config_ptr;
}

StackConfig *read_stack_config_from_widget(GtkWidget *widget)
{
    StackConfig *stack_config_ptr = NULL;
    SAFE_ALLOC(stack_config_ptr, StackConfig, 1);

    StackConfig stack_config = DEFAULT_STACK;

    // Homogeneous
    stack_config.is_homogeneous = gtk_stack_get_homogeneous(GTK_STACK(widget));

    // Transition enabled
    stack_config.is_transition_enabled = gtk_stack_get_transition_type(GTK_STACK(widget)) != GTK_STACK_TRANSITION_TYPE_NONE;

    // Transition duration
    stack_config.transition_duration = gtk_stack_get_transition_duration(GTK_STACK(widget));

    // Transition type
    stack_config.transition_type = gtk_stack_get_transition_type(GTK_STACK(widget));

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    stack_config.dimensions.width = allocation.width;
    stack_config.dimensions.height = allocation.height;

    // Expand
    stack_config.hexpand = gtk_widget_get_hexpand(widget);
    stack_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    stack_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    stack_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    stack_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(stack_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(stack_config.text_color, property_value);

    memcpy(stack_config_ptr, &stack_config, sizeof(StackConfig));

    return stack_config_ptr;
}

gchar *write_stack_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "stack", tabs_number);

    // Get the GtkStack from the view
    GtkStack *stack = GTK_STACK(view->widget);

    // Check if the stack is homogeneous
    gboolean is_homogeneous = gtk_stack_get_homogeneous(stack);
    if (is_homogeneous != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_homogeneous=\"%s\"\n", is_homogeneous ? "true" : "false");
    }

    // Check if transitions are enabled
    gboolean is_transition_enabled = gtk_stack_get_transition_type(stack) != GTK_STACK_TRANSITION_TYPE_NONE;
    if (is_transition_enabled != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_transition_enabled=\"%s\"\n", is_transition_enabled ? "true" : "false");
    }

    // Get the transition duration
    guint transition_duration = gtk_stack_get_transition_duration(stack);
    if (transition_duration != 250) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "transition_duration=\"%d\"\n", transition_duration);
    }

    // Get the transition type
    GtkStackTransitionType transition_type = gtk_stack_get_transition_type(stack);
    if (transition_type != GTK_STACK_TRANSITION_TYPE_CROSSFADE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "transition_type=\"%s\"\n", transition_type == GTK_STACK_TRANSITION_TYPE_NONE ? "none" : transition_type == GTK_STACK_TRANSITION_TYPE_CROSSFADE      ? "crossfade"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT      ? "slide_right"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT       ? "slide_left"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_UP         ? "slide_up"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN       ? "slide_down"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT ? "slide_left_right"
                                                                                                                  : transition_type == GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN    ? "slide_up_down"
                                                                                                                                                                                  : "unknown");
    }

    return "stack";
}