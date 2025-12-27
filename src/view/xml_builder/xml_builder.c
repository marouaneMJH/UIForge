#include "./../../../include/widgets/view/xml_builder/xml_builder.h"

gboolean is_container(View *view)
{
    if (!view->widget)
        return FALSE;

    if (GTK_IS_BUTTON(view->widget))
        return FALSE;

    if (GTK_IS_MENU_ITEM(view->widget) && view->child && GTK_IS_MENU(view->child->widget))
    {
        return TRUE;
    }

    return (GTK_IS_BOX(view->widget) ||
            GTK_IS_FLOW_BOX(view->widget) ||
            GTK_IS_MENU(view->widget) ||
            GTK_IS_MENU_BAR(view->widget) ||
            GTK_IS_PANED(view->widget) ||
            GTK_IS_FIXED(view->widget) ||
            GTK_IS_FRAME(view->widget) ||
            GTK_IS_EXPANDER(view->widget) ||
            GTK_IS_NOTEBOOK(view->widget) ||
            GTK_IS_OVERLAY(view->widget) ||
            GTK_IS_EVENT_BOX(view->widget) ||
            GTK_IS_WINDOW(view->widget) ||
            GTK_IS_GRID(view->widget) ||
            GTK_IS_SCROLLED_WINDOW(view->widget) ||
            GTK_IS_STACK(view->widget));
}

gchar *write_widget_property(FILE *output_file, View *view, int tabs_number)
{
    if (!view || !output_file)
        return "\0";

    // // find widget and write it
    if (GTK_IS_SCROLLED_WINDOW(view->widget))
    {
        return write_scrolled_window_property(output_file, view, tabs_number);
    }
    if (GTK_IS_BOX(view->widget))
    {
        return write_box_property(output_file, view, tabs_number);
    }
    if (GTK_IS_FIXED(view->widget))
        return write_fixed_property(output_file, view, tabs_number);

    if (GTK_IS_RADIO_BUTTON(view->widget))
        return write_radio_button_property(output_file, view, tabs_number);

    if (GTK_IS_STACK(view->widget))
        return write_stack_property(output_file, view, tabs_number);

    if (GTK_IS_NOTEBOOK(view->widget))
        return write_notebook_property(output_file, view, tabs_number);

    if (GTK_IS_SWITCH(view->widget))
        return write_switch_button_property(output_file, view, tabs_number);

    if (GTK_IS_ENTRY(view->widget))
        return write_entry_property(output_file, view, tabs_number);

    if (GTK_IS_LABEL(view->widget))
        return write_label_property(output_file, view, tabs_number);

    if (GTK_IS_SEPARATOR(view->widget))
        return write_separator_property(output_file, view, tabs_number);

    if (GTK_IS_MENU_BAR(view->widget))
        return write_menu_bar_property(output_file, view, tabs_number);

    if (GTK_IS_MENU(view->widget))
        return write_menu_property(output_file, view, tabs_number);

    if (GTK_IS_MENU_ITEM(view->widget))
        return write_menu_item_property(output_file, view, tabs_number);

    if (GTK_IS_SPIN_BUTTON(view->widget))
        return write_spin_button_property(output_file, view, tabs_number);

    if (GTK_IS_FLOW_BOX(view->widget))
        return write_flow_box_property(output_file, view, tabs_number);

    if (GTK_IS_LIST_BOX(view->widget))
        return write_list_box_property(output_file, view, tabs_number);

    if (GTK_IS_GRID(view->widget))
        return write_grid_property(output_file, view, tabs_number);

    if (GTK_IS_PANED(view->widget))
        return write_paned_property(output_file, view, tabs_number);

    if (GTK_IS_IMAGE(view->widget))
        return write_image_property(output_file, view, tabs_number);

    if (GTK_IS_PROGRESS_BAR(view->widget))
        return write_progress_bar_property(output_file, view, tabs_number);

    if (GTK_IS_CHECK_BUTTON(view->widget))
        return write_check_button_property(output_file, view, tabs_number);

    if (GTK_IS_LINK_BUTTON(view->widget))
        return write_link_button_property(output_file, view, tabs_number);

    if (GTK_IS_FRAME(view->widget))
        return write_frame_property(output_file, view, tabs_number);

    if (GTK_IS_TEXT_VIEW(view->widget))
        return write_text_area_property(output_file, view, tabs_number);

    if (GTK_IS_OVERLAY(view->widget))
        return write_overlay_property(output_file, view, tabs_number);

    if (GTK_IS_COMBO_BOX_TEXT(view->widget))
        return write_combo_text_box_property(output_file, view, tabs_number);

    if (GTK_IS_TOGGLE_BUTTON(view->widget))
        return write_toggle_button_property(output_file, view, tabs_number);

    if (GTK_IS_COLOR_BUTTON(view->widget))
        return write_color_button_property(output_file, view, tabs_number);

    if (GTK_IS_EXPANDER(view->widget))
        return write_expander_property(output_file, view, tabs_number);

    if (GTK_IS_EVENT_BOX(view->widget))
        return write_event_box_property(output_file, view, tabs_number);

    if (GTK_IS_WINDOW(view->widget))
        return write_window_property(output_file, view, tabs_number);

    if (GTK_IS_BUTTON(view->widget))
        return write_button_property(output_file, view, tabs_number);

    return "\0";
}

// write child widgets then clos tag
void write_widget_close_tag(FILE *output_file, View *view, gchar *tag, int tabs_number)
{

    if (!output_file || !view || !tag)
    {
        g_critical("write_widget_close_tag: NULL parameter.");
        return;
    }
    // close tag for container
    if (is_container(view))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, ">\n");

        write_widget(output_file, view->child, tabs_number + 1);

        // todo
        // verify the dual behavior widget
        // sometime  act like container and other like widget  menu_item ...

        print_tabs(output_file, tabs_number);
        fprintf(output_file, "</%s>\n", tag);
    }
    // close tag for  normal widget
    else
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "/>\n");
    }
}

// recursive function to write the the tree
void write_widget(FILE *output_file, View *view, int tabs_number)
{

    // exit the function if void
    if (!view || !output_file)
        return;

    // write widget property and child widgets
    char *str = write_widget_property(output_file, view, tabs_number);

    // close tag
    write_widget_close_tag(output_file, view, str, tabs_number);

    // write the brother widget
    write_widget(output_file, view->next, tabs_number);
}

void build_xml(gchar *file_name, gboolean dialog_is_showed)
{
    MessageDialogConfig m = DEFAULT_MESSAGE_DIALOG_CONFIG;

    if (!file_name)
        return;

    FILE *output_file = fopen(file_name, "w");
    if (!output_file)
    {
        if (dialog_is_showed)
        { // Properly format the error message

            char *msg = g_strdup_printf("%s: not opening!", file_name);
            strcpy(m.message, msg);
            GtkWidget *message = create_message_dialog(m);
            g_free(msg); // Free the allocated memory
            show_dialog(message);
        }
        return;
    }

    fprintf(output_file, "<dialog\nid=\"win1\" \n\t width=\"900\"\n\theight=\"500\"\n\ttitle=\"GTK IDE\"\n\ticon_name=\"folder\"\n\thas_header=\"false\"\n\tis_maximized=\"false\"\n>\n");
    root_crud_ui = find_view_by_id("viewer", root_view_global);

    // generate the xml the racine widget xml code
    if (root_crud_ui && root_crud_ui->child)
        write_widget(output_file, root_crud_ui->child, 1);
    // write_widget(output_file, root_view_global, 0); // debug for debuging

    else
    {
        strcpy(m.message, "can't generate UI: void tree\nadd element to the tree");
        GtkWidget *message = create_message_dialog(m);
        show_dialog(message);
        fclose(output_file);
        return;
    }

    fprintf(output_file, "</dialog>");
    fclose(output_file);

    if (dialog_is_showed)
    { // Properly format the success message
        char *msg = g_strdup_printf("Check the output in %s", file_name);
        strcpy(m.message, msg);
        g_free(msg); // Free the allocated memory

        GtkWidget *message = create_message_dialog(m);
        show_dialog(message);
    }
}
