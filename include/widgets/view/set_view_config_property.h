#pragma once
#define SET_SIGNAL_PROPERTY                                                      \
    if (g_strcmp0(property, "on_click") == 0)                                    \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_CLICK;                           \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_toggle") == 0)                              \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_TOGGLE;                          \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_change") == 0)                              \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_CHANGE;                          \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_activate") == 0)                            \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_ACTIVATE;                        \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_delete_event") == 0)                        \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_DELETE_EVENT;                    \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_destroy") == 0)                             \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_DESTROY;                         \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_value_changed") == 0)                       \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_VALUE_CHANGED;                   \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_row_activated") == 0)                       \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_ROW_ACTIVATED;                   \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_key_press") == 0)                           \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_KEY_PRESS;                       \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_button_press") == 0)                        \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_BUTTON_PRESS;                    \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_button_release") == 0)                      \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_BUTTON_RELEASE;                  \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_focus_in") == 0)                            \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_FOCUS_IN;                        \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_focus_out") == 0)                           \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_FOCUS_OUT;                       \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_selection_changed") == 0)                   \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_SELECTION_CHANGED;               \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_popup_menu") == 0)                          \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_POPUP_MENU;                      \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_response") == 0)                            \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_RESPONSE;                        \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }                                                                            \
    else if (g_strcmp0(property, "on_color_set") == 0)                           \
    {                                                                            \
        view_config->signal.event_type = SIG_ON_COLOR_SET;                       \
        g_strlcpy(view_config->signal.sig_handler, value, MAX_SIGNAL_NAME_SIZE); \
    }

#define SET_VIEW_CONFIG_PROPERTY(property, value, view_config)                      \
    if (g_strcmp0(property, "position_x") == 0)                                     \
    {                                                                               \
        view_config->position_x = atoi(value);                                      \
    }                                                                               \
    if (g_strcmp0(property, "position_y") == 0)                                     \
    {                                                                               \
        view_config->position_y = atoi(value);                                      \
    }                                                                               \
    if (g_strcmp0(property, "pack_direction") == 0)                                 \
    {                                                                               \
        view_config->pack_direction = atoi(value);                                  \
    }                                                                               \
    if (g_strcmp0(property, "box_expand") == 0)                                     \
    {                                                                               \
        view_config->box_expand = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;     \
    }                                                                               \
    if (g_strcmp0(property, "box_fill") == 0)                                       \
    {                                                                               \
        view_config->box_fill = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;       \
    }                                                                               \
    if (g_strcmp0(property, "tab_label") == 0)                                      \
    {                                                                               \
        g_strlcpy(view_config->tab_label, value, MAX_LABEL_SIZE);                   \
    }                                                                               \
    if (g_strcmp0(property, "box_padding") == 0)                                    \
    {                                                                               \
        view_config->box_padding = atoi(value);                                     \
    }                                                                               \
    if (g_strcmp0(property, "flow_box_order") == 0)                                 \
    {                                                                               \
        view_config->flow_box_order = atoi(value);                                  \
    }                                                                               \
    if (g_strcmp0(property, "paned_order") == 0)                                    \
    {                                                                               \
        view_config->paned_order = atoi(value);                                     \
    }                                                                               \
    if (g_strcmp0(property, "is_reorderable") == 0)                                 \
    {                                                                               \
        view_config->is_reorderable = g_strcmp0(value, "true") == 0 ? TRUE : FALSE; \
    }                                                                               \
    if (g_strcmp0(property, "row") == 0)                                            \
    {                                                                               \
        view_config->row = atoi(value);                                             \
    }                                                                               \
    if (g_strcmp0(property, "column") == 0)                                         \
    {                                                                               \
        view_config->column = atoi(value);                                          \
    }                                                                               \
    if (g_strcmp0(property, "row_span") == 0)                                       \
    {                                                                               \
        view_config->row_span = atoi(value);                                        \
    }                                                                               \
    if (g_strcmp0(property, "column_span") == 0)                                    \
    {                                                                               \
        view_config->column_span = atoi(value);                                     \
    }                                                                               \
    SET_SIGNAL_PROPERTY; /* configure the signale property */                       \
    if (g_strcmp0(property, "param_1") == 0)                                        \
    {                                                                               \
        g_strlcpy(view_config->param[0], value, MAX_SIGNAL_NAME_SIZE);              \
    }                                                                               \
    if (g_strcmp0(property, "param_2") == 0)                                        \
    {                                                                               \
        g_strlcpy(view_config->param[1], value, MAX_SIGNAL_NAME_SIZE);              \
    }                                                                               \
    if (g_strcmp0(property, "param_3") == 0)                                        \
    {                                                                               \
        g_strlcpy(view_config->param[2], value, MAX_SIGNAL_NAME_SIZE);              \
    }                                                                               \
    if (g_strcmp0(property, "param_4") == 0)                                        \
    {                                                                               \
        g_strlcpy(view_config->param[3], value, MAX_SIGNAL_NAME_SIZE);              \
    }                                                                               \
    if (g_strcmp0(property, "menu_orientation") == 0)                               \
    {                                                                               \
        g_strlcpy(view_config->menu_orientation, value, MAX_SIGNAL_NAME_SIZE);      \
    }                                                                               \
    if (g_strcmp0(property, "menu_top") == 0)                                       \
    {                                                                               \
        view_config->menu_top = atoi(value);                                        \
    }                                                                               \
    if (g_strcmp0(property, "menu_bottom") == 0)                                    \
    {                                                                               \
        view_config->menu_bottom = atoi(value);                                     \
    }                                                                               \
    if (g_strcmp0(property, "menu_left") == 0)                                      \
    {                                                                               \
        view_config->menu_left = atoi(value);                                       \
    }                                                                               \
    if (g_strcmp0(property, "menu_right") == 0)                                     \
    {                                                                               \
        view_config->menu_right = atoi(value);                                      \
    }

#define DFEAULT_VIEW_CONFIG(view_config)                                    \
    do                                                                      \
    {                                                                       \
        view_config->position_x = 0;                                        \
        view_config->position_y = 0;                                        \
        view_config->pack_direction = 1;                                    \
        view_config->box_expand = FALSE;                                    \
        view_config->box_fill = FALSE;                                      \
        view_config->box_padding = 0;                                       \
        view_config->flow_box_order = 0;                                    \
        view_config->paned_order = 0;                                       \
        view_config->group = NULL;                                          \
        view_config->view_id[0] = '\0';                                     \
        view_config->tab_label[0] = '\0';                                   \
        view_config->is_reorderable = TRUE;                                 \
        view_config->row = 0;                                               \
        view_config->column = 0;                                            \
        view_config->row_span = 1;                                          \
        view_config->column_span = 1;                                       \
        strcpy(view_config->menu_orientation, "vertical");                  \
        view_config->menu_top = 0;                                          \
        view_config->menu_bottom = 1;                                       \
        view_config->menu_left = 0;                                         \
        view_config->menu_right = 0;                                        \
                                                                            \
        /* Initialize signal configuration */                               \
        view_config->signal.event_type = SIG_NONE;                          \
        view_config->signal.sig_handler[0] = '\0';                          \
                                                                            \
        /* Clear parameter array */                                         \
        for (int i = 0; i < PARAM_COUNT; view_config->param[i++][0] = '\0') \
            ;                                                               \
    } while (0)
