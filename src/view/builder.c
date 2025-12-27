#include "./../../include/builder.h"
#include "helper/read_tag.h"
#include "helper/link_with_container.h"

// Factory pattern for SOLID principles
typedef View *(*WidgetReaderFunction)(FILE *, View *, gboolean);
typedef View *(*AppWidgetReaderFunction)(FILE *, GtkApplication *, View *, gboolean);

typedef struct
{
    const gchar *tag_name;
    WidgetReaderFunction reader_func;
    AppWidgetReaderFunction app_reader_func;
} WidgetFactory;

// Forward declarations for reader functions
View *read_dialog_tag_wrapper(FILE *index, View *parent_view, gboolean is_relative_container);

// Widget factory registry - adheres to Open/Closed Principle
static const WidgetFactory widget_factories[] = {
    {"window", NULL, (AppWidgetReaderFunction)read_window_tag},
    {"scrolled_window", read_scrolled_window_tag, NULL},
    {"box", read_box_tag, NULL},
    {"fixed", read_fixed_tag, NULL},
    {"stack", read_stack_tag, NULL},
    {"notebook", read_notebook_tag, NULL},
    {"button", read_button_tag, NULL},
    {"switch_button", read_switch_button_tag, NULL},
    {"entry", read_entry_tag, NULL},
    {"label", read_label_tag, NULL},
    {"separator", read_separator_tag, NULL},
    {"menu_bar", read_menu_bar_tag, NULL},
    {"menu", read_menu_tag, NULL},
    {"menu_item", read_menu_item_tag, NULL},
    {"radio_button", read_radio_button_tag, NULL},
    {"spin_button", read_spin_button_tag, NULL},
    {"flow_box", read_flow_box_tag, NULL},
    {"grid", read_grid_tag, NULL},
    {"paned", read_paned_tag, NULL},
    {"image", read_image_tag, NULL},
    {"progress_bar", read_progress_bar_tag, NULL},
    {"check_button", read_check_button_tag, NULL},
    {"link_button", read_link_button_tag, NULL},
    {"frame", read_frame_tag, NULL},
    {"text_area", read_text_area_tag, NULL},
    {"overlay", read_overlay_tag, NULL},
    {"dialog", read_dialog_tag_wrapper, NULL},
    {"combo_text_box", read_combo_text_box_tag, NULL},
    {"toggle_button", read_toggle_button_tag, NULL},
    {"color_button", read_color_button_tag, NULL},
    {"expander", read_expander_tag, NULL},
    {"event_box", read_event_box_tag, NULL},
    {NULL, NULL, NULL} // Sentinel
};

// Helper function to find widget factory - Single Responsibility Principle
static const WidgetFactory *find_widget_factory(const gchar *widget_tag)
{
    if (!widget_tag)
        return NULL;

    for (const WidgetFactory *factory = widget_factories; factory->tag_name; factory++)
    {
        if (g_strcmp0(factory->tag_name, widget_tag) == 0)
        {
            return factory;
        }
    }
    return NULL;
}

// Widget processor function - Single Responsibility Principle
static View *process_widget(const gchar *widget_tag, FILE *index, GtkApplication *app,
                            View *parent_view, gboolean is_relative_container,
                            View **root_view, View **root_menu_bar_view)
{

    const WidgetFactory *factory = find_widget_factory(widget_tag);

    if (!factory)
    {
        g_print("ERROR: Widget << %s >> not found\n", widget_tag);
        return parent_view;
    }

    View *new_view = NULL;

    if (factory->app_reader_func)
    {
        new_view = factory->app_reader_func(index, app, parent_view, is_relative_container);

        // Special handling for root views
        if (g_strcmp0(widget_tag, "window") == 0)
        {
            *root_view = new_view;
            root_view_global = new_view;
        }
    }
    else if (factory->reader_func)
    {
        new_view = factory->reader_func(index, parent_view, is_relative_container);

        // Special handling for specific widgets
        if (g_strcmp0(widget_tag, "menu_bar") == 0)
        {
            *root_menu_bar_view = new_view;
        }
        else if (g_strcmp0(widget_tag, "dialog") == 0)
        {
            *root_view = new_view;
            root_dialog_view_global = new_view;
            g_print("Dialog view readed\n");
        }
    }

    return new_view ? new_view : parent_view;
}

// Wrapper functions for compatibility
View *read_dialog_tag_wrapper(FILE *index, View *parent_view, gboolean is_relative_container)
{
    return read_dialog_tag(index, parent_view, is_relative_container);
}

static gint wid = 0;
void add_command(const gchar *label, gint x, gint y, const gchar *signal_option, View *commands_container, View *root_view)
{

    ViewConfig *view_conf;
    SAFE_ALLOC(view_conf, ViewConfig, 1);

    if (!view_conf || !root_view || !commands_container)
        return;

    strcpy(view_conf->view_id, g_strconcat("cmd-", g_strdup_printf("%d", wid++), NULL));
    view_conf->signal.event_type = SIG_ON_CLICK;
    strcpy(view_conf->signal.sig_handler, "sig_properties_dialog");

    view_conf->position_x = x;
    view_conf->position_y = y;
    strcpy(view_conf->param[0], signal_option);

    ButtonConfig btn_conf = DEFAULT_BUTTON;
    strcpy(btn_conf.label, label);
    GtkWidget *btn_widget = create_button(btn_conf);

    g_signal_connect(G_OBJECT(btn_widget), "clicked", G_CALLBACK(sig_properties_dialog), (ParamNode *)view_conf->param);

    link_with_container(commands_container->widget, btn_widget, view_conf);
    g_print("Command added\n");
}

void add_custom_command(ViewConfig *view_conf, const gchar *label, gint x, gint y, View *commands_container, View *root_view)
{

    if (!view_conf || !root_view || !commands_container)
        return;

    strcpy(view_conf->view_id, g_strconcat("cmd-", g_strdup_printf("%d", wid++), NULL));
    view_conf->signal.event_type = SIG_ON_CLICK;
    // if (view_conf->signal.sig_handler[0] == '\0')
    // {
    // strcpy(view_conf->signal.sig_handler, signal);
    // }

    view_conf->position_x = x;
    view_conf->position_y = y;
    // strcpy(view_conf->param[0], params.params[0]);
    // strcpy(view_conf->param[1], params.params[1]);
    // strcpy(view_conf->param[2], params.params[2]);
    // strcpy(view_conf->param[3], params.params[3]);

    ButtonConfig btn_conf = DEFAULT_BUTTON;
    strcpy(btn_conf.label, label);
    GtkWidget *btn_widget = create_button(btn_conf);

    // g_signal_connect(G_OBJECT(btn_widget), "clicked", G_CALLBACK(sig_properties_dialog), (ParamNode *)view_conf->param);
    connect_signals(create_view(btn_widget, view_conf));
    link_with_container(commands_container->widget, btn_widget, view_conf);
    g_print("Command added\n");
}

View *create_view(GtkWidget *widget, ViewConfig *view_config)
{
    View *view = NULL;
    SAFE_ALLOC(view, View, 1);
    if (!view)
        g_printerr("Failed: create new view failed!\n");

    view->child = NULL;
    view->parent = NULL;
    view->next = NULL;
    view->widget = widget;
    view->view_config = view_config;

    /* Connect to the view widget if existe */
    connect_signals(view);

    return view;
}

gchar *read_tag(FILE *index)
{
    gchar *tag = NULL;
    SAFE_ALLOC(tag, gchar, MAX_TAG_SIZE);

    int i = 0;
    char c;
    // fscanf(index, "%s", tag);
    while ((c = fgetc(index)) != '>')
    {
        if (c == ' ' || c == '\n')
            break;

        tag[i++] = c;
    }

    tag[i] = '\0';
    return tag;
}

// Legacy function kept for compatibility but now uses factory pattern
int get_view_index(gchar *widget_tag)
{
    if (!widget_tag)
        return -1;

    const WidgetFactory *factory = find_widget_factory(widget_tag);
    if (factory)
    {
        // Return index based on position in factory array
        return (int)(factory - widget_factories);
    }

    return -1;
}

static int concurence = 0;
int check_view_id_existance(View *root, gchar *view_id)
{

    if (!root)
        return 0;

    if (g_strcmp0(root->view_config->view_id, view_id) == 0)
        concurence++;

    check_view_id_existance(root->child, view_id);
    check_view_id_existance(root->next, view_id);

    return concurence;
}

View *add_view(View *view, View *relative, gboolean is_relative_container)
{
    if (!view)
        return NULL;
    // g_print("Adding view %s\n", view->view_config->view_id);
    if (!relative)
    {
        g_print("Relative view is null\n");
        return view;
    }

    // Check if the view id is not repeated
    concurence = 0;
    if (check_view_id_existance(root_view_global, view->view_config->view_id) > 0)
    {
        g_print("==========================================================\n");
        g_print("# Error: view_id (%s) is repeated.\n", view->view_config->view_id);
        g_print("==========================================================\n");
    }

    // Group radio buttons
    if (GTK_IS_RADIO_BUTTON(view->widget))
    {

        if (GTK_IS_RADIO_BUTTON(relative->widget) && !relative->view_config->group)
        {
            view->view_config->group = relative->widget;
            radio_button_set_group(view->widget, relative->widget);
        }
        else
        {
            view->view_config->group = relative->view_config->group;
            radio_button_set_group(view->widget, relative->view_config->group);
        }
    }

    // Menu bar and menu items
    if (GTK_IS_MENU_ITEM(view->widget))
    {
        if (GTK_IS_MENU_BAR(relative->widget) || GTK_IS_MENU(relative->widget))
        {
            view->view_config->group = relative->widget;
            if (relative->view_config->menu_orientation[0] != '\0')
            {
                if (g_strcmp0(relative->view_config->menu_orientation, "vertical") == 0)
                    gtk_menu_shell_append(GTK_MENU_SHELL(relative->widget), view->widget);
                else
                    gtk_menu_attach(GTK_MENU(relative->widget), view->widget, view->view_config->menu_left, view->view_config->menu_right, view->view_config->menu_top, view->view_config->menu_bottom);
            }
        }
        else if (relative->view_config->group)
        {
            view->view_config->group = relative->view_config->group;
            if (relative->parent->view_config->menu_orientation[0] != '\0')
            {
                if (g_strcmp0(relative->parent->view_config->menu_orientation, "vertical") == 0)
                    gtk_menu_shell_append(GTK_MENU_SHELL(relative->view_config->group), view->widget);
                else
                    gtk_menu_attach(GTK_MENU(relative->view_config->group), view->widget, view->view_config->menu_left, view->view_config->menu_right, view->view_config->menu_top, view->view_config->menu_bottom);
            }
        }
    }

    if (GTK_IS_MENU(view->widget))
    {
        if (GTK_IS_MENU_ITEM(relative->widget))
        {
            gtk_menu_item_set_submenu(GTK_MENU_ITEM(relative->widget), view->widget);
        }
    }

    // Notebook
    if (view->view_config->tab_label[0] != '\0')
    {
        if (GTK_IS_NOTEBOOK(relative->widget))
        {
            gtk_notebook_append_page(GTK_NOTEBOOK(relative->widget), view->widget, gtk_label_new(view->view_config->tab_label));
            gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(relative->widget), view->widget, view->view_config->is_reorderable);
        }
        else if (relative->parent)
        {
            if (GTK_IS_NOTEBOOK(relative->parent->widget))
            {
                gtk_notebook_append_page(GTK_NOTEBOOK(relative->parent->widget), view->widget, gtk_label_new(view->view_config->tab_label));
                gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(relative->parent->widget), view->widget, view->view_config->is_reorderable);
            }
        }
    }

    if (is_relative_container)
    {
        // g_print("Relative container\n");
        view->parent = relative;
        relative->child = view;

        // printf("RELATIVE PARENT %s IS A CONTAINER FOR: %s\n", relative->view_config->view_id, view->view_config->view_id);
        // Window case
        if (GTK_IS_WINDOW(relative->widget) || GTK_IS_SCROLLED_WINDOW(relative->widget) || GTK_IS_DIALOG(relative->widget))
        {

            if (GTK_IS_DIALOG(relative->widget))
            {
                // g_print("Dialog container\n");
                GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(relative->widget));
                gtk_container_add(GTK_CONTAINER(content_area), view->widget);
                // g_print("Dialog container added\n");
            }
            else
                gtk_container_add(GTK_CONTAINER(relative->widget), view->widget);
            return view;
        }

        link_with_container(relative->widget, view->widget, view->view_config);
        // g_print("Linked with container\n");
    }
    else
    {
        view->parent = relative->parent;
        relative->next = view;

        link_with_container(relative->parent->widget, view->widget, view->view_config);
    }

    // printf("This widget %s is linked to container\n", view->view_config->view_id);
    return view;
}

gboolean is_container_view(FILE *index)
{
    fseek(index, -2, SEEK_CUR);
    gchar c1 = fgetc(index);
    gchar c2 = fgetc(index);
    if (c1 == '/' && c2 == '>')
        return FALSE;
    else if (c2 == '>')
        return TRUE;

    return FALSE;
}

void read_comment(FILE *index)
{
    char c;
    while ((c = fgetc(index)) != '-')
        ;
    if (fgetc(index) != '-')
    {
        fseek(index, -1, SEEK_CUR);
        read_comment(index);
    }
    else if (fgetc(index) != '>')
    {
        fseek(index, -2, SEEK_CUR);
        read_comment(index);
    }
}

View *build_app(GtkApplication *app, View *root_view, const gchar *file_path)
{
    printf("Building app\n");

    // This file is read from the main.c path because this function is called/executed from main.c
    FILE *index = fopen(file_path, MODE);
    if (!index)
    {
        g_printerr("Failed to open index file\n");
        return NULL;
    }

    // !
    g_print("Index file opened\n");

    View *parent_view = root_view;
    View *root_menu_bar_view = NULL;
    gboolean is_relative_container = TRUE;
    gchar *widget_tag = NULL;

    // gchar *view_id = NULL;
    int widget_index = -1;
    char c;

    gboolean stop = FALSE;
    while ((c = fgetc(index)) != EOF && !stop)
    {

        // Ignore comment
        if (c == '#')
        {
            while ((c = fgetc(index)) != '\n')
                ;
            continue;
        }

        if (c == '<')
        {
            char next_char = fgetc(index);
            if (next_char == '/')
            {
                if (!parent_view)
                {
                    ("Error: Parent view is NULL while closing tag\n");
                    break;
                }
                is_relative_container = FALSE;
                parent_view = parent_view->parent;
                // !
                g_print("Parent view changed\n");
                continue;
            }
            else if (next_char == '!')
            {
                read_comment(index);
                continue;
            }
            else
            {
                fseek(index, -1, SEEK_CUR);

                // Read tag and check if exists
                widget_tag = read_tag(index);

                // Get the widget index
                widget_index = get_view_index(widget_tag);
            }

            // Factory pattern replaces switch statement - follows Open/Closed Principle
            parent_view = process_widget(widget_tag, index, app, parent_view,
                                         is_relative_container, &root_view, &root_menu_bar_view);

            // Update container flag
            is_relative_container = is_container_view(index);

            free(widget_tag);
        }
    }

    fclose(index);
    g_print("Index file closed\n");
    if (root_view)
        g_print("Root view created\n");
    return root_view;
}
