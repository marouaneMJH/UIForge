#include "./../../../include/widgets/menu/menu_properties.h"

/*
 * Property configuration dispatcher
 */
ViewConfig *configure_menu_property(MenuConfig *cfg,
                                    ViewConfig *view_config,
                                    const char *property,
                                    const char *value)
{
    if (!cfg || !property || !value)
        return NULL;

    property_registry_dispatch(MenuConfig_registry, cfg, property, value);

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_menu_config(FILE *index, MenuConfig *menu_config)
{
    register_menu_properties();

    return init_generic_config(index,
                               (void *)menu_config,
                               (ConfigurePropertyCallback)configure_menu_property);
}

GtkWidget *create_menu(MenuConfig menu_config)
{
    GtkWidget *menu = gtk_menu_new();

    if (menu_config.accel_group)
        gtk_menu_set_accel_group(GTK_MENU(menu), menu_config.accel_group);

    if (menu_config.accel_path[0])
        gtk_menu_set_accel_path(GTK_MENU(menu), menu_config.accel_path);

    gtk_menu_set_active(GTK_MENU(menu), menu_config.active_index);
    gtk_menu_set_monitor(GTK_MENU(menu), menu_config.monitor_num);
    gtk_menu_set_reserve_toggle_size(GTK_MENU(menu), menu_config.reserve_toggle_size);
    gtk_menu_set_screen(GTK_MENU(menu), menu_config.screen);

    if (menu_config.tooltip[0])
        gtk_widget_set_tooltip_text(menu, menu_config.tooltip);

    if (menu_config.dimensions.width > 0 && menu_config.dimensions.height > 0)
        gtk_widget_set_size_request(menu,
                                    menu_config.dimensions.width,
                                    menu_config.dimensions.height);

    widget_set_colors(menu, menu_config.bg_color, menu_config.text_color);
    widget_set_margins(menu, menu_config.margins);

    gtk_widget_set_hexpand(menu, menu_config.hexpand);
    gtk_widget_set_vexpand(menu, menu_config.vexpand);

    gtk_widget_set_halign(menu, menu_config.halign);
    gtk_widget_set_valign(menu, menu_config.valign);

    return menu;
}

void menu_set_group(GtkWidget *widget, GtkWidget *group) {}

MenuConfig *read_menu_config_from_dialog()
{
    MenuConfig *menu_config_ptr = NULL;
    SAFE_ALLOC(menu_config_ptr, MenuConfig, 1);

    MenuConfig menu_config = DEFAULT_MENU;

    const gchar *tooltip = read_config_value_as_string("tooltip_entry");
    strcpy(menu_config.tooltip, tooltip);

    menu_config.active_index = read_config_value_as_int("active_index_spin");
    menu_config.monitor_num = read_config_value_as_int("monitor_num_spin");
    menu_config.reserve_toggle_size = read_config_value_as_boolean("reserve_toggle_size_switch");

    Dimensions *dimensions = read_dimensions_config();
    menu_config.dimensions.width = dimensions->width;
    menu_config.dimensions.height = dimensions->height;

    Margins *margins = read_margins_config();
    menu_config.margins = *margins;

    menu_config.halign = read_align_config("halign_combo");
    menu_config.valign = read_align_config("valign_combo");

    menu_config.hexpand = read_config_value_as_boolean("hexpand_switch");
    menu_config.vexpand = read_config_value_as_boolean("vexpand_switch");

    strcpy(menu_config.bg_color, read_config_value_as_string("bg_color_entry"));
    strcpy(menu_config.text_color, read_config_value_as_string("color_entry"));

    memcpy(menu_config_ptr, &menu_config, sizeof(MenuConfig));
    return menu_config_ptr;
}

MenuConfig *read_menu_config_from_widget(GtkWidget *widget)
{
    MenuConfig *menu_config_ptr = NULL;
    SAFE_ALLOC(menu_config_ptr, MenuConfig, 1);

    MenuConfig menu_config = DEFAULT_MENU;

    const gchar *tooltip = gtk_widget_get_tooltip_text(widget);
    if (tooltip)
        strcpy(menu_config.tooltip, tooltip);

    GtkWidget *active_item = gtk_menu_get_active(GTK_MENU(widget));
    if (active_item)
    {
        GList *children = gtk_container_get_children(GTK_CONTAINER(widget));
        menu_config.active_index = g_list_index(children, active_item);
        g_list_free(children);
    }

    menu_config.monitor_num = gtk_menu_get_monitor(GTK_MENU(widget));
    menu_config.reserve_toggle_size = gtk_menu_get_reserve_toggle_size(GTK_MENU(widget));

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    menu_config.dimensions.width = allocation.width;
    menu_config.dimensions.height = allocation.height;

    menu_config.hexpand = gtk_widget_get_hexpand(widget);
    menu_config.vexpand = gtk_widget_get_vexpand(widget);

    menu_config.halign = gtk_widget_get_halign(widget);
    menu_config.valign = gtk_widget_get_valign(widget);

    Margins margins;
    widget_get_margins(widget, &margins);
    menu_config.margins = margins;

    gchar *pv = read_bg_color_from_widget(widget);
    if (pv)
        strcpy(menu_config.bg_color, pv);

    pv = read_text_color_from_widget(widget);
    if (pv)
        strcpy(menu_config.text_color, pv);

    memcpy(menu_config_ptr, &menu_config, sizeof(MenuConfig));
    return menu_config_ptr;
}

gchar *write_menu_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    write_widget_tag_style_view_config(output_file, view, "menu", tabs_number);

    GtkMenu *menu = GTK_MENU(view->widget);

    const gchar *accel_path = gtk_menu_get_accel_path(menu);
    if (accel_path && accel_path[0])
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "accel_path=\"%s\"\n", accel_path);
    }

    GtkWidget *active_item = gtk_menu_get_active(menu);
    if (active_item)
    {
        GList *children = gtk_container_get_children(GTK_CONTAINER(menu));
        gint active_index = g_list_index(children, active_item);
        g_list_free(children);

        if (active_index != 0)
        {
            print_tabs(output_file, tabs_number + 1);
            fprintf(output_file, "active_index=\"%d\"\n", active_index);
        }
    }

    gboolean reserve_toggle_size = gtk_menu_get_reserve_toggle_size(menu);
    if (reserve_toggle_size != TRUE)
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file,
                "reserve_toggle_size=\"%s\"\n",
                reserve_toggle_size ? "true" : "false");
    }

    gint monitor_num = gtk_menu_get_monitor(menu);
    if (monitor_num != 0)
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "monitor_num=\"%d\"\n", monitor_num);
    }

    const gchar *tooltip = gtk_widget_get_tooltip_text(GTK_WIDGET(menu));
    if (tooltip && tooltip[0])
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "tooltip=\"%s\"\n", tooltip);
    }

    return "menu";
}
