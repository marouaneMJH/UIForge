#include "./../../include/builder.h"
#include "helper/read_tag.h"
#include "helper/link_with_container.h"

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

void add_custom_command(ViewConfig* view_conf, const gchar *label, gint x, gint y, View *commands_container, View *root_view)
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

static void sig_entry_activate(GtkWidget *entry, gpointer data)
{
    g_print("Hellow entry");
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

int get_view_index(gchar *widget_tag) // Why FILE *index
{
    if (!widget_tag)
        return -1;

    if (g_strcmp0(widget_tag, "window") == 0)
        return WindowTag;

    if (g_strcmp0(widget_tag, "scrolled_window") == 0)
        return ScrolledWindowTag;

    if (g_strcmp0(widget_tag, "box") == 0)
        return BoxTag;

    if (g_strcmp0(widget_tag, "fixed") == 0)
        return FixedTag;

    if (g_strcmp0(widget_tag, "stack") == 0)
        return StackTag;

    if (g_strcmp0(widget_tag, "notebook") == 0)
        return NotebookTag;

    if (g_strcmp0(widget_tag, "button") == 0)
        return ButtonTag;

    if (g_strcmp0(widget_tag, "entry") == 0)
        return EntryTag;

    if (g_strcmp0(widget_tag, "label") == 0)
        return LabelTag;

    if (g_strcmp0(widget_tag, "separator") == 0)
        return SeparatorTag;

    if (g_strcmp0(widget_tag, "menu_bar") == 0)
        return MenuBarTag;

    if (g_strcmp0(widget_tag, "menu") == 0)
        return MenuTag;

    if (g_strcmp0(widget_tag, "menu_item") == 0)
        return MenuItemTag;

    if (g_strcmp0(widget_tag, "radio_button") == 0)
        return RadioButtonTag;

    if (g_strcmp0(widget_tag, "image") == 0)
        return ImageTag;

    if (g_strcmp0(widget_tag, "spin_button") == 0)
        return SpinButtonTag;

    if (g_strcmp0(widget_tag, "flow_box") == 0)
        return FlowBoxTag;

    if (g_strcmp0(widget_tag, "list_box") == 0)
        return ListBoxTag;

    if (g_strcmp0(widget_tag, "grid") == 0)
        return GridTag;

    if (g_strcmp0(widget_tag, "paned") == 0)
        return PanedTag;

    if (g_strcmp0(widget_tag, "link_button") == 0)
        return LinkButtonTag;

    if (g_strcmp0(widget_tag, "switch_button") == 0)
        return SwitchButtonTag;

    if (g_strcmp0(widget_tag, "check_button") == 0)
        return CheckButtonTag;

    if (g_strcmp0(widget_tag, "progress_bar") == 0)
        return ProgressBarTag;
    if (g_strcmp0(widget_tag, "combo_text_box") == 0)
        return ComboTextBoxTag;

    if (g_strcmp0(widget_tag, "dialog") == 0)
        return DialogTag;

    if (g_strcmp0(widget_tag, "frame") == 0)
        return FrameTag;

    if (g_strcmp0(widget_tag, "text_area") == 0)
        return TextAreaTag;

    if (g_strcmp0(widget_tag, "overlay") == 0)
        return OverlayTag;

    if (g_strcmp0(widget_tag, "combo_text_box") == 0)
        return ComboTextBoxTag;

    if (g_strcmp0(widget_tag, "toggle_button") == 0)
        return ToggleButtonTag;

    if (g_strcmp0(widget_tag, "color_button") == 0)
        return ColorButtonTag;

    if (g_strcmp0(widget_tag, "expander") == 0)
        return ExpanderTag;

    if (g_strcmp0(widget_tag, "event_box") == 0)
        return EventBoxTag;

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
                is_relative_container = FALSE;
                parent_view = parent_view->parent;
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

                // TODO: if the tag = NULL then ignore the tag and Display an error message

                // Get the widget index
                widget_index = get_view_index(widget_tag);
            }

            switch (widget_index)
            {
            case WindowTag:
                // Read window tag
                parent_view = read_window_tag(index, app, parent_view, is_relative_container);

                // Set window as root view parent to be returned
                root_view = parent_view;

                root_view_global = parent_view;

                // Update container flag
                is_relative_container = is_container_view(index);

                break;
            case BoxTag:

                parent_view = read_box_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case FixedTag:

                parent_view = read_fixed_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case FlowBoxTag:

                parent_view = read_flow_box_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case PanedTag:
                parent_view = read_paned_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case NotebookTag:
                parent_view = read_notebook_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case GridTag:
                parent_view = read_grid_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case EntryTag:

                parent_view = read_entry_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case RadioButtonTag:

                parent_view = read_radio_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;

            case ButtonTag:

                parent_view = read_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;

            case ImageTag:
                parent_view = read_image_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;

            case MenuBarTag:
                parent_view = read_menu_bar_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                root_menu_bar_view = parent_view;

                break;
            case MenuTag:
                parent_view = read_menu_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case MenuItemTag:

                parent_view = read_menu_item_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                // TODO: Check why it works
                // parent_view = menu_item_view;

                // gtk_menu_shell_append(GTK_MENU_SHELL(root_menu_bar_view->widget), menu_item_view->widget);

                break;
            case SpinButtonTag:
                parent_view = read_spin_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case LabelTag:
                parent_view = read_label_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case SeparatorTag:
                parent_view = read_separator_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case CheckButtonTag:
                parent_view = read_check_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case LinkButtonTag:
                parent_view = read_link_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case ScrolledWindowTag:
                parent_view = read_scrolled_window_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case ProgressBarTag:

                parent_view = read_progress_bar_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);

                break;
            case SwitchButtonTag:
                parent_view = read_switch_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case StackTag:
                parent_view = read_stack_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case FrameTag:

                parent_view = read_frame_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            case TextAreaTag:

                parent_view = read_text_area_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;
            case OverlayTag:

                parent_view = read_overlay_tag(index, parent_view, is_relative_container);
            case DialogTag:
                parent_view = read_dialog_tag(index, parent_view, is_relative_container);
                root_view = parent_view;
                root_dialog_view_global = parent_view;
                is_relative_container = is_container_view(index);
                g_print("Dialog view readed\n");
                break;

            case ComboTextBoxTag:

                parent_view = read_combo_text_box_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            case ToggleButtonTag:
                parent_view = read_toggle_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            case ColorButtonTag:
                parent_view = read_color_button_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            case ExpanderTag:
                parent_view = read_expander_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            case EventBoxTag:
                parent_view = read_event_box_tag(index, parent_view, is_relative_container);
                is_relative_container = is_container_view(index);
                break;

            // TODO : Complete other widgets
            default:
                stop = TRUE;
                fclose(index);
                g_print("ERROR: %d => Widget << %s >> not found\n", widget_index, widget_tag);
                //  exit(EXIT_FAILURE);
                break;
            } // end of switch

            free(widget_tag);
        }
    }

    fclose(index);
    g_print("Index file closed\n");
    if (root_view)
        g_print("Root view created\n");
    return root_view;
}
