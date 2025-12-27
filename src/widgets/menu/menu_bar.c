#include "./../../../include/widgets/menu/menu_bar.h"

ViewConfig *configure_menu_bar_property(MenuBarConfig *menu_bar_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!menu_bar_config || !property || !value)
        return NULL;

    if (g_strcmp0(property, "pack_direction") == 0)
    {
        if (g_strcmp0(value, "LTR") == 0)
            menu_bar_config->pack_direction = GTK_PACK_DIRECTION_LTR;

        if (g_strcmp0(value, "RTL") == 0)
            menu_bar_config->pack_direction = GTK_PACK_DIRECTION_RTL;

        if (g_strcmp0(value, "TTB") == 0)
            menu_bar_config->pack_direction = GTK_PACK_DIRECTION_TTB;

        if (g_strcmp0(value, "BTT") == 0)
            menu_bar_config->pack_direction = GTK_PACK_DIRECTION_BTT;
    }

    if (g_strcmp0(property, "tooltip") == 0)
        strcpy(menu_bar_config->tooltip, value);

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        menu_bar_config->margins.top = atoi(value);

    if (g_strcmp0(property, "margin_bottom") == 0)
        menu_bar_config->margins.bottom = atoi(value);

    if (g_strcmp0(property, "margin_left") == 0)
        menu_bar_config->margins.start = atoi(value);

    if (g_strcmp0(property, "margin_right") == 0)
        menu_bar_config->margins.end = atoi(value);

    // Dimensions
    if (g_strcmp0(property, "width") == 0)
        menu_bar_config->dimensions.width = atoi(value);

    if (g_strcmp0(property, "height") == 0)
        menu_bar_config->dimensions.height = atoi(value);

    // Colors
    if (g_strcmp0(property, "bg_color") == 0)
        strcpy(menu_bar_config->bg_color, value);

    if (g_strcmp0(property, "text_color") == 0)
        strcpy(menu_bar_config->text_color, value);

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            menu_bar_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            menu_bar_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            menu_bar_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            menu_bar_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            menu_bar_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            menu_bar_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            menu_bar_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            menu_bar_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_menu_bar_config(FILE *index, MenuBarConfig *menu_bar_config)
{
    return init_generic_config(index, (void *)menu_bar_config, (ConfigurePropertyCallback)configure_menu_bar_property);
}

GtkWidget *create_menu_bar(MenuBarConfig menu_bar_config)
{
    GtkWidget *menu_bar = NULL;

    // Creation
    menu_bar = gtk_menu_bar_new();

    gtk_menu_bar_set_pack_direction(GTK_MENU_BAR(menu_bar), menu_bar_config.pack_direction);

    // Set the tooltip
    gtk_widget_set_tooltip_text(menu_bar, menu_bar_config.tooltip);

    // Set dimensions
    if (menu_bar_config.dimensions.width > 0 && menu_bar_config.dimensions.height > 0)
    {
        gtk_widget_set_size_request(menu_bar, menu_bar_config.dimensions.width, menu_bar_config.dimensions.height);
    }

    // Set colors
    widget_set_colors(menu_bar, menu_bar_config.bg_color, menu_bar_config.text_color);

    widget_set_margins(menu_bar, menu_bar_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(menu_bar, menu_bar_config.hexpand);
    gtk_widget_set_vexpand(menu_bar, menu_bar_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(menu_bar, menu_bar_config.halign);
    gtk_widget_set_valign(menu_bar, menu_bar_config.valign);

    return menu_bar;
}

MenuBarConfig *read_menu_bar_config_from_dialog()
{
    MenuBarConfig *menu_bar_config_ptr = NULL;
    SAFE_ALLOC(menu_bar_config_ptr, MenuBarConfig, 1);

    MenuBarConfig menu_bar_config = DEFAULT_MENU_BAR;

    // Pack direction
    const gchar *orientation = read_config_value_as_string("pack_direction_combo");
    if (strcasecmp(orientation, "RTL") == 0)
        menu_bar_config.pack_direction = GTK_PACK_DIRECTION_RTL;
    else if (strcasecmp(orientation, "TTB") == 0)
        menu_bar_config.pack_direction = GTK_PACK_DIRECTION_TTB;
    else if (strcasecmp(orientation, "BTT") == 0)
        menu_bar_config.pack_direction = GTK_PACK_DIRECTION_BTT;
    else
        menu_bar_config.pack_direction = GTK_PACK_DIRECTION_LTR;

    // Tooltip
    const gchar *tooltip = read_config_value_as_string("tooltip_entry");
    strcpy(menu_bar_config.tooltip, tooltip);

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    menu_bar_config.dimensions.width = dimensions->width;
    menu_bar_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    menu_bar_config.margins.top = margins->top;
    menu_bar_config.margins.bottom = margins->bottom;
    menu_bar_config.margins.start = margins->start;
    menu_bar_config.margins.end = margins->end;

    // HAlign
    menu_bar_config.halign = read_align_config("halign_combo");

    // VAlign
    menu_bar_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    menu_bar_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    menu_bar_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(menu_bar_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(menu_bar_config.text_color, text_color);

    memcpy(menu_bar_config_ptr, &menu_bar_config, sizeof(MenuBarConfig));
    return menu_bar_config_ptr;
}

MenuBarConfig *read_menu_bar_config_from_widget(GtkWidget *widget)
{
    MenuBarConfig *menu_bar_config_ptr = NULL;
    SAFE_ALLOC(menu_bar_config_ptr, MenuBarConfig, 1);

    MenuBarConfig menu_bar_config = DEFAULT_MENU_BAR;

    // Pack direction
    GtkPackDirection pack_direction = gtk_menu_bar_get_pack_direction(GTK_MENU_BAR(widget));
    menu_bar_config.pack_direction = pack_direction;

    // Tooltip
    const gchar *tooltip = gtk_widget_get_tooltip_text(widget);
    if (tooltip)
        strcpy(menu_bar_config.tooltip, tooltip);

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    menu_bar_config.dimensions.width = allocation.width;
    menu_bar_config.dimensions.height = allocation.height;

    // Expand
    menu_bar_config.hexpand = gtk_widget_get_hexpand(widget);
    menu_bar_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    menu_bar_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    menu_bar_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    menu_bar_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(menu_bar_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(menu_bar_config.text_color, property_value);

    memcpy(menu_bar_config_ptr, &menu_bar_config, sizeof(MenuBarConfig));

    return menu_bar_config_ptr;
}

gchar *write_menu_bar_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "menu_bar", tabs_number);

    // Get the GtkMenuBar from the view
    GtkMenuBar *menu_bar = GTK_MENU_BAR(view->widget);

    // Get the tooltip text
    const gchar *tooltip = gtk_widget_get_tooltip_text(GTK_WIDGET(menu_bar));
    if (g_strcmp0(tooltip, "\0") != 0) // Check if the tooltip text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "tooltip=\"%s\"\n", tooltip);
    }

    // Get the pack direction
    GtkPackDirection pack_direction = gtk_menu_bar_get_pack_direction(menu_bar);
    if (pack_direction != GTK_PACK_DIRECTION_LTR) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "pack_direction=\"%s\"\n", pack_direction == GTK_PACK_DIRECTION_LTR ? "ltr" : pack_direction == GTK_PACK_DIRECTION_RTL ? "rtl"
                                                                                                       : pack_direction == GTK_PACK_DIRECTION_TTB   ? "ttb"
                                                                                                       : pack_direction == GTK_PACK_DIRECTION_BTT   ? "btt"
                                                                                                                                                    : "unknown");
    }

    return "menu_bar";
}