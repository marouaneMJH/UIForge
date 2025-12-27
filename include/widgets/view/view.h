#ifndef VIEW_H
#define VIEW_H

#include "./../../global.h"
#include "signals.h"
#include "set_view_config_property.h"

typedef struct
{
    gchar view_id[MAX_VIEW_ID_SIZE];

    // Fixed container
    gint position_x;
    gint position_y;

    // Box container
    gint pack_direction;
    gboolean box_expand;
    gboolean box_fill;
    gint box_padding;

    // FlowBox container
    gint flow_box_order;

    // Paned container
    // add1: 0 or add2: 1
    gint paned_order;

    // Ex: radio button
    GtkWidget *group;

    // NoteBook container properties
    
    // tab label in Notebook container
    gchar tab_label[MAX_LABEL_SIZE];
    gboolean is_reorderable;

    // Grid container properties
    gint row;
    gint column;
    gint row_span;
    gint column_span;

    // Signals
    // todo to delete it


    SignalConfig signal; // the signale name and handler

    // Params of Signals
    gchar param[PARAM_COUNT][MAX_SIGNAL_NAME_SIZE]; // First function parameter

    gchar menu_orientation[MAX_LABEL_SIZE];
    gint menu_top;
    gint menu_bottom;
    gint menu_left;
    gint menu_right;

} ViewConfig;

typedef struct VIEW
{
    GtkWidget *widget;
    struct VIEW *parent;
    struct VIEW *child;
    struct VIEW *next;
    ViewConfig *view_config;
} View;

#endif