#include "./../../include/widgets/text_area.h"

ViewConfig *configure_text_area_property(TextAreaConfig *text_area_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!text_area_config || !property || !value)
        return NULL;

    if (strcmp(property, "font_size") == 0)
    {
        text_area_config->font_size = atoi(value);
    }
    else if (strcmp(property, "text_color") == 0)
    {
        strncpy(text_area_config->text_color, value, MAX_COLOR_SIZE);
    }
    else if (strcmp(property, "background_color") == 0)
    {
        strncpy(text_area_config->bg_color, value, MAX_COLOR_SIZE);
    }
    else if (strcmp(property, "is_editable") == 0)
    {
        text_area_config->is_editable = (strcmp(value, "true") == 0) ? TRUE : FALSE;
    }
    else if (strcmp(property, "width") == 0)
    {
        text_area_config->dimensions.width = atoi(value);
    }
    else if (strcmp(property, "height") == 0)
    {
        text_area_config->dimensions.height = atoi(value);
    }
    else if (strcmp(property, "wrap_mode") == 0)
    {
        if (strcmp(value, "none") == 0)
            text_area_config->wrap_mode = GTK_WRAP_NONE;
        else if (strcmp(value, "char") == 0)
            text_area_config->wrap_mode = GTK_WRAP_CHAR;
        else if (strcmp(value, "word") == 0)
            text_area_config->wrap_mode = GTK_WRAP_WORD;
        else if (strcmp(value, "word-char") == 0)
            text_area_config->wrap_mode = GTK_WRAP_WORD_CHAR;
    }

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        text_area_config->margins.top = atoi(value);

    if (g_strcmp0(property, "margin_bottom") == 0)
        text_area_config->margins.bottom = atoi(value);

    if (g_strcmp0(property, "margin_left") == 0)
        text_area_config->margins.start = atoi(value);

    if (g_strcmp0(property, "margin_right") == 0)
        text_area_config->margins.end = atoi(value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            text_area_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            text_area_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            text_area_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            text_area_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            text_area_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            text_area_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            text_area_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            text_area_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_text_area_config(FILE *index, TextAreaConfig *text_area_config)
{
    return init_generic_config(index, (void *)text_area_config, (ConfigurePropertyCallback)configure_text_area_property);
}

GtkWidget *create_text_area(TextAreaConfig text_area_config)
{

    GtkWidget *text_view_widget = gtk_text_view_new();

    /* Apply properties */
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_widget), text_area_config.wrap_mode);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_widget), text_area_config.is_editable);

    if (text_area_config.font_size > 0)
        widget_set_font_size(text_view_widget, text_area_config.font_size);

    widget_set_colors(text_view_widget, text_area_config.bg_color, text_area_config.text_color);

    // gtk_container_add(GTK_CONTAINER(scrolled_window), text_view_widget);
    if (text_area_config.dimensions.width > 0 && text_area_config.dimensions.height > 0)
        set_widget_size(text_view_widget, text_area_config.dimensions);

    widget_set_margins(text_view_widget, text_area_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(text_view_widget, text_area_config.hexpand);
    gtk_widget_set_vexpand(text_view_widget, text_area_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(text_view_widget, text_area_config.halign);
    gtk_widget_set_valign(text_view_widget, text_area_config.valign);

    return text_view_widget;
}

TextAreaConfig *read_text_area_config_from_dialog()
{
    TextAreaConfig *text_area_config_ptr = NULL;
    SAFE_ALLOC(text_area_config_ptr, TextAreaConfig, 1);

    TextAreaConfig text_area_config = DEFAULT_TEXT_AREA;

    // Font size
    gint font_size = read_config_value_as_int("font_size_spin");
    text_area_config.font_size = font_size;

    // Wrap mode
    const gchar *wrap_mode = read_config_value_as_string("wrap_mode_combo");
    if (strcasecmp(wrap_mode, "none") == 0)
        text_area_config.wrap_mode = GTK_WRAP_NONE;
    else if (strcasecmp(wrap_mode, "word") == 0)
        text_area_config.wrap_mode = GTK_WRAP_WORD;
    else if (strcasecmp(wrap_mode, "word char") == 0)
        text_area_config.wrap_mode = GTK_WRAP_WORD_CHAR;
    else
        text_area_config.wrap_mode = GTK_WRAP_CHAR;

    // Editable
    gboolean is_editable = read_config_value_as_boolean("editable_switch");
    text_area_config.is_editable = is_editable;

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    text_area_config.dimensions.width = dimensions->width;
    text_area_config.dimensions.height = dimensions->height;

    // // Margins
    // Margins *margins = read_margins_config();
    // text_area_config.margins.top = margins->top;
    // text_area_config.margins.bottom = margins->bottom;
    // text_area_config.margins.start = margins->start;
    // text_area_config.margins.end = margins->end;

    // HAlign
    text_area_config.halign = read_align_config("halign_combo");

    // VAlign
    text_area_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    text_area_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    text_area_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(text_area_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(text_area_config.text_color, text_color);

    memcpy(text_area_config_ptr, &text_area_config, sizeof(TextAreaConfig));
    return text_area_config_ptr;
}

TextAreaConfig *read_text_area_config_from_widget(GtkWidget *widget)
{
    TextAreaConfig *text_area_config_ptr = NULL;
    SAFE_ALLOC(text_area_config_ptr, TextAreaConfig, 1);

    TextAreaConfig text_area_config = DEFAULT_TEXT_AREA;

    // Font size
    PangoContext *pcontext = gtk_widget_get_pango_context(widget);
    PangoFontDescription *font_desc = pango_context_get_font_description(pcontext);
    // const char *family = font_desc ? pango_font_description_get_family(font_desc) : "default";
    int font_size = font_desc ? (pango_font_description_get_size(font_desc) / PANGO_SCALE) : 0;

    // Font size
    text_area_config.font_size = font_size;

    // Wrap mode
    GtkWrapMode wrap_mode = gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(widget));
    text_area_config.wrap_mode = wrap_mode;

    // Editable
    gboolean is_editable = gtk_text_view_get_editable(GTK_TEXT_VIEW(widget));
    text_area_config.is_editable = is_editable;

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    text_area_config.dimensions.width = allocation.width;
    text_area_config.dimensions.height = allocation.height;

    // Expand
    text_area_config.hexpand = gtk_widget_get_hexpand(widget);
    text_area_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    text_area_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    text_area_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    text_area_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(text_area_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(text_area_config.text_color, property_value);

    memcpy(text_area_config_ptr, &text_area_config, sizeof(TextAreaConfig));

    return text_area_config_ptr;
}

gchar *write_text_area_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "text_area", tabs_number);

    // Get the GtkTextView from the view
    GtkTextView *text_view = GTK_TEXT_VIEW(view->widget);

    // Check if the text area is editable
    gboolean is_editable = gtk_text_view_get_editable(text_view);
    if (is_editable != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_editable=\"%s\"\n", is_editable ? "true" : "false");
    }

    // Get the wrap mode
    GtkWrapMode wrap_mode = gtk_text_view_get_wrap_mode(text_view);
    if (wrap_mode != GTK_WRAP_WORD) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "wrap_mode=\"%s\"\n", wrap_mode == GTK_WRAP_NONE ? "none" : wrap_mode == GTK_WRAP_CHAR    ? "char"
                                                                                     : wrap_mode == GTK_WRAP_WORD      ? "word"
                                                                                     : wrap_mode == GTK_WRAP_WORD_CHAR ? "word_char"
                                                                                                                       : "unknown");
    }

    return "text_area";
}