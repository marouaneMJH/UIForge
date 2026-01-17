#ifndef MENU_H
#define MENU_H

#include "./../../global.h"

#define MAX_TITLE_SIZE 50
#define MAX_ACCEL_PATH_SIZE 100

/*  default values */
#define DEFAULT_MENU_DIMENSIONS \
    {                           \
        .height = 0,            \
        .width = 0}

#define DEFAULT_MENU_MARGIN \
    {                       \
        .bottom = 0,        \
        .top = 0,           \
        .start = 0,         \
        .end = 0}

#define DEFAULT_MENU                           \
    {                                          \
        .accel_group = NULL,                   \
        .accel_path = "\0",                    \
        .active_index = 0,                     \
        .reserve_toggle_size = TRUE,           \
        .monitor_num = 0,                      \
        .screen = NULL,                        \
        .tooltip = "\0",                       \
        .dimensions = DEFAULT_MENU_DIMENSIONS, \
        .margins = DEFAULT_MENU_MARGIN,        \
        .hexpand = FALSE,                      \
        .vexpand = FALSE,                      \
        .halign = GTK_ALIGN_FILL,              \
        .valign = GTK_ALIGN_FILL,              \
        .bg_color = "\0",                      \
        .text_color = "\0"}
// .margin = DEFAULT_MENU_MARGIN,

typedef struct
{
    guint active_index;           // Activate programatically a menu
    gboolean reserve_toggle_size; // Reserve the space for checkbox/radio button ...
    guint monitor_num;            // Associate the monitor in multiple monitors setup
    gchar tooltip[MAX_TOOLTIP_SIZE];

    // gchar title[MAX_TITLE_SIZE]; // Add title in case of tear-off menu
    GtkAccelGroup *accel_group; // Add accel path (keyboard shortcuts)
    GdkScreen *screen;          // Associate to the parent screen or else
    gchar accel_path[MAX_ACCEL_PATH_SIZE];

    gboolean hexpand;
    gboolean vexpand;
    GtkAlign halign;
    GtkAlign valign;
    Dimensions dimensions;
    Margins margins;
    gchar bg_color[MAX_COLOR_SIZE];
    gchar text_color[MAX_COLOR_SIZE];

} MenuConfig;

ViewConfig *init_menu_config(FILE *index, MenuConfig *menu_config);

GtkWidget *create_menu(MenuConfig menu_config);

void menu_set_group(GtkWidget *widget, GtkWidget *group);

MenuConfig *read_menu_config_from_dialog();

MenuConfig *read_menu_config_from_widget(GtkWidget *widget);

gchar *write_menu_property(FILE *output_file, View *view, int tabs_number);

#endif