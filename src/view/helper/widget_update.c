#include "./../../../include/builder.h"
#include "./../../../include/widgets/View/widget_update.h"

View *temp_parent_view = NULL;

void rebuild_graph(View **temp_root, View *view, View *updated_view, gchar *view_id, gboolean relative_container)
{
    if (!view || !updated_view)
        return;

    // Ref the widget to prevent destruction
    g_object_ref(view->widget);

    // Remove from parent
    gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(view->widget)), view->widget);

    View *new_view = create_view(view->widget, view->view_config);
    if (g_strcmp0(view->view_config->view_id, view_id))
    {
        temp_parent_view = add_view(new_view, temp_parent_view, relative_container);
        g_print("Old view is done: %s\n", temp_parent_view->view_config->view_id);
    }
    else
    {
        temp_parent_view = add_view(updated_view, temp_parent_view, relative_container);
        g_print("Updated view is done: %s\n", temp_parent_view->view_config->view_id);
        temp_parent_view->view_config->box_expand ? g_print("BOX EXPAND ====> TRUE\n") : g_print("BOX EXPAND ====> FALSE\n");
    }

    // Unref after adding
    // g_object_unref(view->widget);

    g_print("Add is done: %s\n", temp_parent_view->view_config->view_id);
    (*temp_root) = temp_parent_view;

    // relative_container = check_relative_container((*temp_root)->widget);

    if (view->child)
        rebuild_graph(temp_root, view->child, updated_view, view_id, TRUE);

    if (view->next)
        rebuild_graph(temp_root, view->next, updated_view, view_id, FALSE);

    else
        temp_parent_view = temp_parent_view->parent;
}

void destroy_dialog_input(gchar *view_id)
{
    View *target = find_view_by_id(view_id, root_dialog_view_global);
    if (!target)
    {
        g_print("Error: ==> cannot found the view: %s\n", view_id);
        return;
    }

    gtk_widget_destroy(target->widget);
    g_free(target->view_config);
    g_free(target);
}

void disable_dialog_input(gchar *view_id)
{
    View *target = find_view_by_id(view_id, root_dialog_view_global);
    if (!target)
    {
        g_print("Error: ==> cannot found the view: %s\n", view_id);
        return;
    }

    gtk_widget_set_sensitive(target->widget, FALSE);
}

View *apply_update(View *updated_view, ViewConfig *view_config)
{
    View *viewer = find_view_by_id("viewer", root_view_global);
    if (!viewer)
    {
        g_print("ERROR: ==> cannot find the viewer\n");
        return NULL;
    }

    if (!viewer->child)
    {
        g_print("ERROR: ==> VIEWER HAS NO CHILD\n");
        return NULL;
    }

    View *temp_root = NULL;
    rebuild_graph(&temp_root, viewer->child, updated_view, view_config->view_id, check_relative_container(viewer->widget));

    if (!temp_root)
    {
        g_print("ERROR: ==> TEMP ROOT NOT COMPLETED\n");
        return NULL;
    }

    // Add the the new graph to viewer
    g_print("DEBUG: ==> TEMP ROOT TREE\n");
    while (temp_root->parent)
        temp_root = temp_root->parent;
    add_view(temp_root, viewer, TRUE);
    print_graph_to_debug(viewer);
    gtk_widget_show_all(gtk_widget_get_toplevel(temp_root->widget));

    return temp_root;
}

void set_current_view_config_to_dialog(ViewConfig *view_config)
{
    if (!view_config)
        return;
    write_view_config_to_dialog(view_config);
    disable_dialog_input("view_id_entry");
    disable_dialog_input("scope_back_combo");
}

// set current config to dialog
// containers
void set_current_scrolled_window_config_to_dialog(ScrolledWindowConfig *scrolled_window_config)
{

    // // Horizontal adjustment
    // write_adjustment_config("hadjustment_spin", scrolled_window_config->hadjustment);

    // // Vertical adjustment
    // write_adjustment_config("vadjustment_spin", scrolled_window_config->vadjustment);

    // Scroll policy
    switch (scrolled_window_config->h_policy)
    {
    case GTK_POLICY_AUTOMATIC:
        write_config_value_as_combo_index("h_scrollbar_policy_combo", 0);
        break;
    case GTK_POLICY_ALWAYS:
        write_config_value_as_combo_index("h_scrollbar_policy_combo", 1);
        break;
    case GTK_POLICY_NEVER:
        write_config_value_as_combo_index("h_scrollbar_policy_combo", 2);
        break;
    default:
        g_print("Unknown scroll policy\n");
        break;
    }

    switch (scrolled_window_config->v_policy)
    {
    case GTK_POLICY_AUTOMATIC:
        write_config_value_as_combo_index("v_scrollbar_policy_combo", 0);
        break;
    case GTK_POLICY_ALWAYS:
        write_config_value_as_combo_index("v_scrollbar_policy_combo", 1);
        break;
    case GTK_POLICY_NEVER:
        write_config_value_as_combo_index("v_scrollbar_policy_combo", 2);
        break;
    default:
        g_print("Unknown scroll policy\n");
        break;
    }

    // Dimensions
    write_dimensions_config(scrolled_window_config->dimensions);

    // Margins
    write_margins_config(scrolled_window_config->margins);

    // Align
    write_align_config(scrolled_window_config->halign, scrolled_window_config->valign);

    // Expand
    write_expand_config(scrolled_window_config->hexpand, scrolled_window_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", scrolled_window_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", scrolled_window_config->text_color);
}

void set_current_box_config_to_dialog(BoxConfig *box_config)
{

    // Orientation
    // write_orientation_config("orientation_combo", box_config->orientation);
    write_config_value_as_combo_index("orientation_combo", box_config->orientation == GTK_ORIENTATION_VERTICAL ? 0 : 1);

    // Spacing
    write_config_value_as_int("spacing_spin", box_config->spacing);

    // Homogeneous
    write_config_value_as_boolean("homogeneous_switch", box_config->homogeneous);

    // Baseline position
    switch (box_config->baseline_position)
    {
    case GTK_BASELINE_POSITION_CENTER:
        write_config_value_as_combo_index("baseline_combo", 0);
        break;
    case GTK_BASELINE_POSITION_TOP:
        write_config_value_as_combo_index("baseline_combo", 1);
        break;
    case GTK_BASELINE_POSITION_BOTTOM:
        write_config_value_as_combo_index("baseline_combo", 2);
        break;
    default:
        write_config_value_as_combo_index("baseline_combo", 0);
        g_print("Unknown baseline position\n");
        break;
    }

    // Dimensions
    write_dimensions_config(box_config->dimensions);

    // Margins
    write_margins_config(box_config->margins);

    // Align
    write_align_config(box_config->halign, box_config->valign);

    // Expand
    write_expand_config(box_config->hexpand, box_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", box_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", box_config->text_color);

    // Background image
    write_config_value_as_string("bg_image_entry", box_config->bg_image);
}

void set_current_fixed_config_to_dialog(FixedConfig *fixed_config)
{

    // Dimensions
    write_dimensions_config(fixed_config->dimensions);

    // Margins
    write_margins_config(fixed_config->margins);

    // Align
    write_align_config(fixed_config->halign, fixed_config->valign);

    // Expand
    write_expand_config(fixed_config->hexpand, fixed_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", fixed_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", fixed_config->text_color);
}

void set_current_flow_box_config_to_dialog(FlowBoxConfig *flow_box_config)
{

    // Min children per line
    write_config_value_as_int("min_children_per_line_spin", flow_box_config->min_childern_per_line);

    // Max children per line
    write_config_value_as_int("max_children_per_line_spin", flow_box_config->max_childern_per_line);

    // Column spacing
    write_config_value_as_int("column_spacing_spin", flow_box_config->column_spacing);

    // Row spacing
    write_config_value_as_int("row_spacing_spin", flow_box_config->row_spacing);

    // Homogeneous
    write_config_value_as_boolean("homogeneous_switch", flow_box_config->is_homogeneous);

    // Selection mode
    switch (flow_box_config->selection_mode)
    {
    case GTK_SELECTION_SINGLE:
        write_config_value_as_combo_index("selection_mode_combo", 0);
        break;
    case GTK_SELECTION_BROWSE:
        write_config_value_as_combo_index("selection_mode_combo", 1);
        break;
    case GTK_SELECTION_MULTIPLE:
        write_config_value_as_combo_index("selection_mode_combo", 2);
        break;
    case GTK_SELECTION_NONE:
        write_config_value_as_combo_index("selection_mode_combo", 3);
        break;
    default:
        g_print("Unknown selection mode\n");
        break;
    }

    // TODO: Horizontal adjustment

    // TODO: Vertical adjustment
    // write_adjustment_config("vadjustment_spin", flow_box_config->vadjustment);

    // Dimensions
    write_dimensions_config(flow_box_config->dimensions);

    // Margins
    write_margins_config(flow_box_config->margins);

    // Align
    write_align_config(flow_box_config->halign, flow_box_config->valign);

    // Expand
    write_expand_config(flow_box_config->hexpand, flow_box_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", flow_box_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", flow_box_config->text_color);
}

void set_current_frame_config_to_dialog(FrameConfig *frame_config)
{

    // Label
    write_config_value_as_string("label_entry", frame_config->label);

    // Label alignment
    write_config_value_as_double("label_yalign_spin", frame_config->label_yalign);
    write_config_value_as_double("label_xalign_spin", frame_config->label_xalign);

    // Shadow type
    switch (frame_config->shadow_type)
    {
    case GTK_SHADOW_IN:
        write_config_value_as_combo_index("shadow_type_combo", 0);
        break;
    case GTK_SHADOW_OUT:
        write_config_value_as_combo_index("shadow_type_combo", 1);
        break;
    case GTK_SHADOW_ETCHED_IN:
        write_config_value_as_combo_index("shadow_type_combo", 2);
        break;
    case GTK_SHADOW_ETCHED_OUT:
        write_config_value_as_combo_index("shadow_type_combo", 3);
        break;
    case GTK_SHADOW_NONE:
        write_config_value_as_combo_index("shadow_type_combo", 4);
        break;
    default:
        g_print("Unknown shadow type\n");
        break;
    }

    // Dimensions
    write_dimensions_config(frame_config->dimensions);

    // Margins
    write_margins_config(frame_config->margins);

    // Align
    write_align_config(frame_config->halign, frame_config->valign);

    // Expand
    write_expand_config(frame_config->hexpand, frame_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", frame_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", frame_config->text_color);
}

void set_current_grid_config_to_dialog(GridConfig *grid_config)
{

    // Row spacing
    write_config_value_as_int("row_spacing_spin", grid_config->row_spacing);

    // Column spacing
    write_config_value_as_int("column_spacing_spin", grid_config->column_spacing);

    // Column homogeneous
    write_config_value_as_boolean("column_homogeneous_switch", grid_config->column_homogeneous);

    // Row homogeneous
    write_config_value_as_boolean("row_homogeneous_switch", grid_config->row_homogeneous);

    // Dimensions
    write_dimensions_config(grid_config->dimensions);

    // Margins
    write_margins_config(grid_config->margins);

    // Align
    write_align_config(grid_config->halign, grid_config->valign);

    // Expand
    write_expand_config(grid_config->hexpand, grid_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", grid_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", grid_config->text_color);
}

void set_current_notebook_config_to_dialog(NotebookConfig *notebook_config)
{

    // If there is another notebook with the same group name they can exchange the tabs by drop and drag
    write_config_value_as_string("group_name_entry", notebook_config->group_name);

    // Select the current page
    write_config_value_as_int("current_page_spin", notebook_config->current_page);

    // Enable or disable scrollable
    write_config_value_as_boolean("is_scrollable_switch", notebook_config->is_scrollable);

    // Show or hide border
    write_config_value_as_boolean("show_border_switch", notebook_config->show_border);

    // Show or hide tabs
    write_config_value_as_boolean("show_tabs_switch", notebook_config->show_tabs);

    // Position of tabs
    switch (notebook_config->tab_position)
    {
    case GTK_POS_TOP:
        write_config_value_as_combo_index("tab_position_combo", 0);
        break;
    case GTK_POS_LEFT:
        write_config_value_as_combo_index("tab_position_combo", 1);
        break;
    case GTK_POS_RIGHT:
        write_config_value_as_combo_index("tab_position_combo", 2);
        break;
    case GTK_POS_BOTTOM:
        write_config_value_as_combo_index("tab_position_combo", 3);
        break;
    default:
        g_print("Unknown tab position\n");
        break;
    }

    // Dimensions
    write_dimensions_config(notebook_config->dimensions);

    // Margins
    write_margins_config(notebook_config->margins);

    // Align
    write_align_config(notebook_config->halign, notebook_config->valign);

    // Expand
    write_expand_config(notebook_config->hexpand, notebook_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", notebook_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", notebook_config->text_color);
}

void set_current_overlay_config_to_dialog(OverlayConfig *overlay_config)
{

    // Opacity
    // write_config_value_as_double("opacity_spin", overlay_config->opacity);

    // Border radius
    write_config_value_as_int("border_radius_spin", overlay_config->border_radius);

    // Dimensions
    write_dimensions_config(overlay_config->dimensions);

    // Margins
    write_margins_config(overlay_config->margins);

    // Align
    write_align_config(overlay_config->halign, overlay_config->valign);

    // Expand
    write_expand_config(overlay_config->hexpand, overlay_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", overlay_config->bg_color);

    // Text color
    write_config_value_as_string("bg_image_entry", overlay_config->bg_image);
}

void set_current_paned_config_to_dialog(PanedConfig *paned_config)
{

    // Define orientation
    write_config_value_as_combo_index("orientation_combo", paned_config->orientation == GTK_ORIENTATION_VERTICAL ? 0 : 1);

    // Set paned position of the separator
    write_config_value_as_int("position_spin", paned_config->position);

    // Enable or disable wide handle
    write_config_value_as_boolean("is_wide_switch", paned_config->is_wide);

    // Dimensions
    write_dimensions_config(paned_config->dimensions);

    // Margins
    write_margins_config(paned_config->margins);

    // Align
    write_align_config(paned_config->halign, paned_config->valign);

    // Expand
    write_expand_config(paned_config->hexpand, paned_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", paned_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", paned_config->text_color);
}

void set_current_stack_config_to_dialog(StackConfig *stack_config)
{

    // Homogeneous
    write_config_value_as_boolean("homogeneous_switch", stack_config->is_homogeneous);

    // Transition enabled
    write_config_value_as_boolean("transition_enabled_switch", stack_config->is_transition_enabled);

    // Transition duration
    write_config_value_as_int("transition_duration_spin", stack_config->transition_duration);

    // Transition type
    switch (stack_config->transition_type)
    {
    case GTK_STACK_TRANSITION_TYPE_CROSSFADE:
        write_config_value_as_combo_index("transition_type_combo", 0);
        break;
    case GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT:
        write_config_value_as_combo_index("transition_type_combo", 1);
        break;
    case GTK_STACK_TRANSITION_TYPE_OVER_LEFT_RIGHT:
        write_config_value_as_combo_index("transition_type_combo", 2);
        break;
    case GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN:
        write_config_value_as_combo_index("transition_type_combo", 3);
        break;
    case GTK_STACK_TRANSITION_TYPE_OVER_UP_DOWN:
        write_config_value_as_combo_index("transition_type_combo", 4);
        break;
    case GTK_STACK_TRANSITION_TYPE_NONE:
        write_config_value_as_combo_index("transition_type_combo", 5);
        break;
    default:
        g_print("Unknown transition type\n");
        break;
    }

    // Dimensions
    write_dimensions_config(stack_config->dimensions);

    // Margins
    write_margins_config(stack_config->margins);

    // Align
    write_align_config(stack_config->halign, stack_config->valign);

    // Expand
    write_expand_config(stack_config->hexpand, stack_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", stack_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", stack_config->text_color);
}

// simple widgets
void set_current_button_config_to_dialog(ButtonConfig *button_config)
{
    // Label
    write_config_value_as_string("label_entry", button_config->label);

    // Sensitive
    write_config_value_as_boolean("sensitive_switch", button_config->is_sensitive);

    // Visible
    write_config_value_as_boolean("visible_switch", button_config->is_visible);

    // Tooltip
    write_config_value_as_string("tooltip_entry", button_config->tooltip);

    // Icon path
    write_config_value_as_string("icon_path_entry", button_config->icon_path);

    // Icon dimensions
    write_config_value_as_int("icon_width_spin", button_config->icon_dimensions.width);
    write_config_value_as_int("icon_height_spin", button_config->icon_dimensions.height);

    // Always show image
    write_config_value_as_boolean("always_show_image_switch", button_config->always_show_image);

    // Use underline
    write_config_value_as_boolean("use_underline_switch", button_config->use_underline);

    // Icon position
    switch (button_config->icon_position)
    {
    case GTK_POS_LEFT:
        write_config_value_as_combo_index("icon_position_combo", 0);
        break;
    case GTK_POS_TOP:
        write_config_value_as_combo_index("icon_position_combo", 1);
        break;
    case GTK_POS_RIGHT:
        write_config_value_as_combo_index("icon_position_combo", 2);
        break;
    case GTK_POS_BOTTOM:
        write_position_config("icon_position_combo", 3);
        break;
    default:
        g_print("Unknown icon position\n");
        break;
    }

    // Font size
    write_config_value_as_int("font_size_spin", button_config->font_size);

    // Dimensions
    write_dimensions_config(button_config->dimensions);

    // Margins
    write_margins_config(button_config->margins);

    // Align
    write_align_config(button_config->halign, button_config->valign);

    // Expand
    write_expand_config(button_config->hexpand, button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", button_config->color);

    // Background image
    write_config_value_as_string("bg_image_entry", button_config->bg_image);
}

void set_current_check_button_config_to_dialog(CheckButtonConfig *check_button_config)
{

    // Label
    write_config_value_as_string("label_entry", check_button_config->label);

    // Tooltip
    write_config_value_as_string("tooltip_entry", check_button_config->label);

    // Active state
    write_config_value_as_boolean("active_switch", check_button_config->is_active);

    // Inconsistent state
    write_config_value_as_boolean("inconsistent_switch", check_button_config->is_inconsistent);

    // Use underline
    write_config_value_as_boolean("underline_switch", check_button_config->use_underline);

    // Dimensions
    write_dimensions_config(check_button_config->dimensions);

    // Margins
    write_margins_config(check_button_config->margins);

    // Align
    write_align_config(check_button_config->halign, check_button_config->valign);

    // Expand
    write_expand_config(check_button_config->hexpand, check_button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", check_button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", check_button_config->text_color);
}

void set_current_color_button_config_to_dialog(ColorButtonConfig *color_button_config)
{

    // Sensitive
    write_config_value_as_boolean("sensitive_switch", color_button_config->is_sensitive);

    // Visible
    write_config_value_as_boolean("visible_switch", color_button_config->is_visible);

    // Tooltip
    write_config_value_as_string("tooltip_entry", color_button_config->tooltip);

    // Dimensions
    write_dimensions_config(color_button_config->dimensions);

    // Margins
    write_margins_config(color_button_config->margins);

    // Align
    write_align_config(color_button_config->halign, color_button_config->valign);

    // Expand
    write_expand_config(color_button_config->hexpand, color_button_config->vexpand);
}

void set_current_link_button_config_to_dialog(LinkButtonConfig *link_config)
{

    // URI
    write_config_value_as_string("uri_entry", link_config->uri);

    // Label
    write_config_value_as_string("label_entry", link_config->label);

    // Tooltip
    write_config_value_as_string("tooltip_entry", link_config->tooltip);

    // Visited state
    write_config_value_as_boolean("visited_switch", link_config->is_visited);

    // Dimensions
    write_dimensions_config(link_config->dimensions);

    // Margins
    write_margins_config(link_config->margins);

    // Align
    write_align_config(link_config->halign, link_config->valign);

    // Expand
    write_expand_config(link_config->hexpand, link_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", link_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", link_config->text_color);
}

void set_current_radio_button_config_to_dialog(RadioButtonConfig *radio_button_config)
{

    // Label
    write_config_value_as_string("label_entry", radio_button_config->label);

    // Icon name
    write_config_value_as_string("icon_name_entry", radio_button_config->icon_name);

    // Tooltip
    write_config_value_as_string("tooltip_entry", radio_button_config->tooltip);

    // Is group
    write_config_value_as_boolean("group_switch", radio_button_config->is_group);

    // Is mnemonic
    write_config_value_as_boolean("mnemonic_switch", radio_button_config->is_mnemonic);

    // Is selected
    write_config_value_as_boolean("selected_switch", radio_button_config->is_selected);

    // Is button mode
    // write_config_value_as_boolean("button_mode_switch", radio_button_config->is_button_mode);

    // Is inconsistent
    write_config_value_as_boolean("inconsistent_switch", radio_button_config->is_inconsistent);

    // Use underline
    write_config_value_as_boolean("use_underline_switch", radio_button_config->use_underline);

    // Icon position
    switch (radio_button_config->icon_position)
    {
    case GTK_POS_LEFT:
        write_config_value_as_combo_index("icon_position_combo", 0);
        break;
    case GTK_POS_TOP:
        write_config_value_as_combo_index("icon_position_combo", 1);
        break;
    case GTK_POS_RIGHT:
        write_config_value_as_combo_index("icon_position_combo", 2);
        break;
    case GTK_POS_BOTTOM:
        write_config_value_as_combo_index("icon_position_combo", 3);
        break;
    default:
        g_print("Unknown icon position\n");
        break;
    }

    // Dimensions
    write_dimensions_config(radio_button_config->dimensions);

    // Margins
    write_margins_config(radio_button_config->margins);

    // Align
    write_align_config(radio_button_config->halign, radio_button_config->valign);

    // Expand
    write_expand_config(radio_button_config->hexpand, radio_button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", radio_button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", radio_button_config->text_color);
}

void set_current_spin_button_config_to_dialog(SpinButtonConfig *spin_button_config)
{

    // Min value
    write_config_value_as_double("min_spin", spin_button_config->min);

    // Max value
    write_config_value_as_double("max_spin", spin_button_config->max);

    // Step value
    write_config_value_as_double("step_spin", spin_button_config->step);

    // Initial value
    write_config_value_as_double("initial_value_spin", spin_button_config->initial_value);

    // Decimal places
    write_config_value_as_int("decimal_spin", spin_button_config->decimal);

    // Numeric
    write_config_value_as_boolean("numeric_switch", spin_button_config->is_numeric);

    // Digits
    write_config_value_as_boolean("digits_switch", spin_button_config->is_digits);

    // Dimensions
    write_dimensions_config(spin_button_config->dimensions);

    // Margins
    write_margins_config(spin_button_config->margins);

    // Align
    write_align_config(spin_button_config->halign, spin_button_config->valign);

    // Expand
    write_expand_config(spin_button_config->hexpand, spin_button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", spin_button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", spin_button_config->text_color);
}

void set_current_switch_button_config_to_dialog(SwitchButtonConfig *switch_button_config)
{

    // Tooltip
    write_config_value_as_string("tooltip_entry", switch_button_config->tooltip);

    // Active state
    write_config_value_as_boolean("active_switch", switch_button_config->is_active);

    // Visible state
    write_config_value_as_boolean("visible_switch", switch_button_config->is_visible);

    // State
    write_config_value_as_boolean("state_switch", switch_button_config->state);

    // Dimensions
    write_dimensions_config(switch_button_config->dimensions);

    // Margins
    write_margins_config(switch_button_config->margins);

    // Align
    write_align_config(switch_button_config->halign, switch_button_config->valign);

    // Expand
    write_expand_config(switch_button_config->hexpand, switch_button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", switch_button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", switch_button_config->text_color);
}

void set_current_toggle_button_config_to_dialog(ToggleButtonConfig *toggle_button_config)
{

    // Label
    write_config_value_as_string("label_entry", toggle_button_config->label);

    // Tooltip
    write_config_value_as_string("tooltip_entry", toggle_button_config->tooltip);

    // Mnemonic
    write_config_value_as_boolean("mnemonic_switch", toggle_button_config->is_mnemonic);

    // Active state
    write_config_value_as_boolean("active_switch", toggle_button_config->is_active);

    // Visible state
    write_config_value_as_boolean("visible_switch", toggle_button_config->is_visible);

    // Mode
    write_config_value_as_boolean("mode_switch", toggle_button_config->mode);

    // Inconsistent state
    write_config_value_as_boolean("inconsistent_switch", toggle_button_config->is_inconsistent);

    // Dimensions
    write_dimensions_config(toggle_button_config->dimensions);

    // Margins
    write_margins_config(toggle_button_config->margins);

    // Align
    write_align_config(toggle_button_config->halign, toggle_button_config->valign);

    // Expand
    write_expand_config(toggle_button_config->hexpand, toggle_button_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", toggle_button_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", toggle_button_config->text_color);
}

void set_current_menu_bar_config_to_dialog(MenuBarConfig *menu_bar_config)
{

    // Tooltip
    write_config_value_as_string("tooltip_entry", menu_bar_config->tooltip);

    // Pack direction
    switch (menu_bar_config->pack_direction)
    {
    case GTK_PACK_DIRECTION_LTR:
        write_config_value_as_combo_index("pack_direction_combo", 0);
        break;
    case GTK_PACK_DIRECTION_RTL:
        write_config_value_as_combo_index("pack_direction_combo", 1);
        break;
    case GTK_PACK_DIRECTION_TTB:
        write_config_value_as_combo_index("pack_direction_combo", 2);
        break;
    case GTK_PACK_DIRECTION_BTT:
        write_config_value_as_combo_index("pack_direction_combo", 3);
        break;
    default:
        g_print("Unknown pack direction\n");
        break;
    }

    // Dimensions
    write_dimensions_config(menu_bar_config->dimensions);

    // Margins
    write_margins_config(menu_bar_config->margins);

    // Align
    write_align_config(menu_bar_config->halign, menu_bar_config->valign);

    // Expand
    write_expand_config(menu_bar_config->hexpand, menu_bar_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", menu_bar_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", menu_bar_config->text_color);
}

void set_current_menu_item_config_to_dialog(MenuItemConfig *menu_item_config)
{

    // Label
    write_config_value_as_string("label_entry", menu_item_config->label);

    // Accel path
    // write_config_value_as_string("accel_path_entry", menu_item_config->accel_path);

    // Reserve indicator
    write_config_value_as_boolean("reserve_indicator_switch", menu_item_config->reserve_indicator);

    // Use underline
    write_config_value_as_boolean("use_underline_switch", menu_item_config->use_underline);

    // Mnemonic
    write_config_value_as_boolean("mnemonic_switch", menu_item_config->is_mnemonic);

    // Tooltip
    write_config_value_as_string("tooltip_entry", menu_item_config->tooltip);

    // Dimensions
    write_dimensions_config(menu_item_config->dimensions);

    // Margins
    write_margins_config(menu_item_config->margins);

    // Align
    write_align_config(menu_item_config->halign, menu_item_config->valign);

    // Expand
    write_expand_config(menu_item_config->hexpand, menu_item_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", menu_item_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", menu_item_config->text_color);
}

void set_current_menu_config_to_dialog(MenuConfig *menu_config)
{
    // Active index
    write_config_value_as_int("active_index_spin", menu_config->active_index);

    // Reserve toggle size
    write_config_value_as_boolean("reserve_toggle_size_switch", menu_config->reserve_toggle_size);

    // Monitor number
    write_config_value_as_int("monitor_num_spin", menu_config->monitor_num);

    // Tooltip
    write_config_value_as_string("tooltip_entry", menu_config->tooltip);

    // Dimensions
    write_dimensions_config(menu_config->dimensions);

    // Margins
    write_margins_config(menu_config->margins);

    // Align
    write_align_config(menu_config->halign, menu_config->valign);

    // Expand
    write_expand_config(menu_config->hexpand, menu_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", menu_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", menu_config->text_color);
}

void set_current_entry_config_to_dialog(EntryConfig *entry_config)
{
    /* Basic properties */
    write_config_value_as_string("text_entry", entry_config->text);
    write_config_value_as_string("placeholder_text_entry", entry_config->placeholder_text);
    write_config_value_as_boolean("visible_switch", entry_config->is_visible);
    switch (entry_config->purpose)
    {
    case GTK_INPUT_PURPOSE_FREE_FORM:
        write_config_value_as_combo_index("purpose_combo", 0);
        break;
    case GTK_INPUT_PURPOSE_ALPHA:
        write_config_value_as_combo_index("purpose_combo", 1);
        break;
    case GTK_INPUT_PURPOSE_DIGITS:
        write_config_value_as_combo_index("purpose_combo", 2);
        break;
    case GTK_INPUT_PURPOSE_NUMBER:
        write_config_value_as_combo_index("purpose_combo", 3);
        break;
    case GTK_INPUT_PURPOSE_PHONE:
        write_config_value_as_combo_index("purpose_combo", 4);
        break;
    case GTK_INPUT_PURPOSE_URL:
        write_config_value_as_combo_index("purpose_combo", 5);
        break;
    case GTK_INPUT_PURPOSE_EMAIL:
        write_config_value_as_combo_index("purpose_combo", 6);
        break;
    case GTK_INPUT_PURPOSE_NAME:
        write_config_value_as_combo_index("purpose_combo", 7);
        break;
    case GTK_INPUT_PURPOSE_PASSWORD:
        write_config_value_as_combo_index("purpose_combo", 8);
        break;
    case GTK_INPUT_PURPOSE_PIN:
        write_config_value_as_combo_index("purpose_combo", 9);
        break;
    default:
        g_print("Unknown input purpose\n");
        break;
    }

    /* Entry appearance */
    write_config_value_as_boolean("delete_icon_switch", entry_config->has_delete_icon);
    write_config_value_as_boolean("frame_switch", entry_config->has_frame);

    // write_config_value_as_double("opacity_spin", entry_config->opacity);

    /* Text handling */
    write_config_value_as_boolean("overwrite_mode_switch", entry_config->overwrite_mode);
    write_config_value_as_int("max_length_spin", entry_config->max_length);
    write_config_value_as_double("alignment_spin", entry_config->alignment);

    /* Progress and completion */
    write_config_value_as_double("progress_fraction_spin", entry_config->progress_fraction);
    write_config_value_as_double("progress_pulse_step_spin", entry_config->progress_pulse_step);

    /* Activation */
    write_config_value_as_boolean("activates_default_switch", entry_config->activates_default);

    // Dimensions
    write_dimensions_config(entry_config->dimensions);

    // Margins
    write_margins_config(entry_config->margins);

    // Align
    write_align_config(entry_config->halign, entry_config->valign);

    // Expand
    write_expand_config(entry_config->hexpand, entry_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", entry_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", entry_config->text_color);
}

void set_current_image_config_to_dialog(ImageConfig *image_config)
{

    // Image type
    switch (image_config->type)
    {
    case IMAGE_PIXBUF:
        write_config_value_as_combo_index("type_combo", 0);
        break;
    case IMAGE_ICON:
        write_config_value_as_combo_index("type_combo", 1);
        break;
    case IMAGE_ICON_NAME:
        write_config_value_as_combo_index("type_combo", 2);
        break;
    case IMAGE_FILE:
        write_config_value_as_combo_index("type_combo", 3);
        break;
    case IMAGE_ANIMATION:
        write_config_value_as_combo_index("type_combo", 4);
        break;
    case IMAGE_RESOURCE:
        write_config_value_as_combo_index("type_combo", 5);
        break;
    default:
        g_print("Unknown image type\n");
        break;
    }

    // Image path
    write_config_value_as_string("path_entry", image_config->path);

    // Image opacity
    // write_config_value_as_double("opacity_spin", image_config->opacity);

    // Dimensions
    write_dimensions_config(image_config->dimensions);

    // Margins
    write_margins_config(image_config->margins);

    // Align
    write_align_config(image_config->halign, image_config->valign);

    // Expand
    write_expand_config(image_config->hexpand, image_config->vexpand);
}

void set_current_label_config_to_dialog(LabelConfig *label_config)
{

    /* Text Content */
    write_config_value_as_string("label_text_entry", label_config->label_text);
    write_config_value_as_string("font_family_entry", label_config->font_family);

    write_config_value_as_boolean("markup_switch", label_config->is_markup);
    write_config_value_as_boolean("underline_switch", label_config->is_underline);

    /* Appearance */
    write_config_value_as_combo_index("jtype_combo", label_config->jtype);
    write_config_value_as_combo_index("ellipsize_combo", label_config->ellipsize);

    write_config_value_as_boolean("wrap_switch", label_config->is_wrap);
    write_config_value_as_boolean("selectable_switch", label_config->is_selectable);

    write_config_value_as_int("font_size_spin", label_config->font_size);
    write_config_value_as_double("xalign_spin", label_config->xalign);
    write_config_value_as_double("yalign_spin", label_config->yalign);

    // Dimensions
    write_dimensions_config(label_config->dimensions);

    // Margins
    write_margins_config(label_config->margins);

    // Align
    write_align_config(label_config->halign, label_config->valign);

    // Expand
    write_expand_config(label_config->hexpand, label_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", label_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", label_config->text_color);
}

void set_current_progress_bar_config_to_dialog(ProgressBarConfig *progress_bar_config)
{

    // Text
    write_config_value_as_string("text_entry", progress_bar_config->text);

    // Progress fraction
    write_config_value_as_double("progress_fraction_spin", progress_bar_config->progress_fraction);

    // Progress pulse step
    write_config_value_as_double("progress_pulse_step_spin", progress_bar_config->progress_pulse_step);

    // Text visibility
    write_config_value_as_boolean("text_visible_switch", progress_bar_config->is_text_visible);

    // Inverted
    write_config_value_as_boolean("inverted_switch", progress_bar_config->is_inverted);

    // Ellipsize mode
    switch (progress_bar_config->ellipsize)
    {
    case PANGO_ELLIPSIZE_END:
        write_config_value_as_combo_index("ellipsize_combo", 0);
        break;
    case PANGO_ELLIPSIZE_START:
        write_config_value_as_combo_index("ellipsize_combo", 1);
        break;
    case PANGO_ELLIPSIZE_MIDDLE:
        write_config_value_as_combo_index("ellipsize_combo", 2);
        break;
    case PANGO_ELLIPSIZE_NONE:
        write_config_value_as_combo_index("ellipsize_combo", 3);
        break;
    default:
        g_print("Unknown ellipsize mode\n");
        break;
    }

    // Opacity
    // write_config_value_as_double("opacity_spin", progress_bar_config->opacity);

    // Dimensions
    write_dimensions_config(progress_bar_config->dimensions);

    // Margins
    write_margins_config(progress_bar_config->margins);

    // Align
    write_align_config(progress_bar_config->halign, progress_bar_config->valign);

    // Expand
    write_expand_config(progress_bar_config->hexpand, progress_bar_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", progress_bar_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", progress_bar_config->text_color);
}

void set_current_separator_config_to_dialog(SeparatorConfig *separator_config)
{
    // Tooltip
    write_config_value_as_string("tooltip_entry", separator_config->tooltip);

    // Orientation
    write_config_value_as_combo_index("orientation_combo", separator_config->orientation == GTK_ORIENTATION_VERTICAL ? 0 : 1);

    // Visible
    write_config_value_as_boolean("visible_switch", separator_config->is_visible);

    // Dimensions
    write_dimensions_config(separator_config->dimensions);

    // Margins
    write_margins_config(separator_config->margins);

    // Align
    write_align_config(separator_config->halign, separator_config->valign);

    // Expand
    write_expand_config(separator_config->hexpand, separator_config->vexpand);
}

void set_current_text_area_config_to_dialog(TextAreaConfig *text_area_config)
{

    // Font size
    write_config_value_as_int("font_size_spin", text_area_config->font_size);

    // Editable state
    write_config_value_as_boolean("editable_switch", text_area_config->is_editable);

    // Wrap mode
    switch (text_area_config->wrap_mode)
    {
    case GTK_WRAP_WORD:
        write_config_value_as_combo_index("wrap_mode_combo", 0);
        break;
    case GTK_WRAP_CHAR:
        write_config_value_as_combo_index("wrap_mode_combo", 1);
        break;
    case GTK_WRAP_WORD_CHAR:
        write_config_value_as_combo_index("wrap_mode_combo", 2);
        break;
    case GTK_WRAP_NONE:
        write_config_value_as_combo_index("wrap_mode_combo", 3);
        break;
    default:
        g_print("Unknown wrap mode\n");
        break;
    }

    // Dimensions
    write_dimensions_config(text_area_config->dimensions);

    // Margins
    write_margins_config(text_area_config->margins);

    // Align
    write_align_config(text_area_config->halign, text_area_config->valign);

    // Expand
    write_expand_config(text_area_config->hexpand, text_area_config->vexpand);

    // Background color
    write_config_value_as_string("bg_color_entry", text_area_config->bg_color);

    // Text color
    write_config_value_as_string("color_entry", text_area_config->text_color);
}

void set_current_combo_text_box_config_to_dialog(ComboTextBoxConfig *combo_text_box_config)
{

    // Basic properties
    // write_config_value_as_ptr_array("options_array", combo_text_box_config->options);

    write_config_value_as_boolean("has_entry_switch", combo_text_box_config->has_entry);
    write_config_value_as_string("placeholder_text_entry", combo_text_box_config->placeholder_text);
    write_config_value_as_string("default_value_entry", combo_text_box_config->default_value);
    write_config_value_as_int("default_index_spin", combo_text_box_config->default_index);
    write_config_value_as_int("wrap_width_spin", combo_text_box_config->wrap_width);

    // Behavior properties
    write_config_value_as_boolean("popup_fixed_width_switch", combo_text_box_config->popup_fixed_width);
    write_config_value_as_int("popup_shown_rows_spin", combo_text_box_config->popup_shown_rows);

    switch (combo_text_box_config->type.type_counter)
    {
    case NONE:
        write_config_value_as_combo_index("type_combo", 0);
        break;
    case COUNTER:
        write_config_value_as_combo_index("type_combo", 1);
        break;
    case YEAR:
        write_config_value_as_combo_index("type_combo", 2);
        break;
    case DAY:
        write_config_value_as_combo_index("type_combo", 3);
        break;
    case MONTH:
        write_config_value_as_combo_index("type_combo", 4);
        break;
    default:
        g_print("Unknown combo text box type\n");
        break;
    }

    // TODO: Display options separated with semi-colon ;
    // gchar *options = NULL;
    // if (combo_text_box_config->type.type_counter == NONE &&
    //     combo_text_box_config->options)
    // {
    //     for (int i = 0; i < combo_text_box_config->options->len; i++)
    //     {
    //         ComboTextBoxOption *option = g_ptr_array_index(combo_text_box_config->options, i);
    //         gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_text_box), option->key, option->value);
    //     }
    // }

    write_config_value_as_int("start_spin", combo_text_box_config->type.start);
    write_config_value_as_int("end_spin", combo_text_box_config->type.end);

    write_config_value_as_boolean("editable_switch", combo_text_box_config->is_editable);

    // Dimensions
    write_dimensions_config(combo_text_box_config->dimensions);

    // Margins
    write_margins_config(combo_text_box_config->margins);

    // Align
    write_align_config(combo_text_box_config->halign, combo_text_box_config->valign);

    // Expand
    write_expand_config(combo_text_box_config->hexpand, combo_text_box_config->vexpand);
}

// prepare the update dialog and show it
// containers
// GtkWidget *prepare_update_scrolled_window_config(View *target_view)
// {
//     ScrolledWindowConfig *scrolled_window_config = read_scrolled_window_config_from_widget(target_view->widget);
//     build_app(root_app, NULL, SCROLLED_WINDOW_PROPERTIES_DIALOG_TXT);
//     set_current_scrolled_window_config_to_dialog(scrolled_window_config);
//     set_current_view_config_to_dialog(target_view->view_config);
//     return root_dialog_view_global->widget;
// }

GtkWidget *prepare_update_scrolled_window_config(View *target_view)
{
    ScrolledWindowConfig *scrolled_window_config = read_scrolled_window_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, SCROLLED_WINDOW_PROPERTIES_DIALOG_TXT);
    set_current_scrolled_window_config_to_dialog(scrolled_window_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_box_config(View *target_view)
{
    BoxConfig *box_config = read_box_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, BOX_PROPERTIES_DIALOG_TXT);
    set_current_box_config_to_dialog(box_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_fixed_config(View *target_view)
{
    FixedConfig *fixed_config = read_fixed_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, FIXED_PROPERTIES_DIALOG_TXT);
    set_current_fixed_config_to_dialog(fixed_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_flow_box_config(View *target_view)
{
    FlowBoxConfig *flow_box_config = read_flow_box_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, FLOW_BOX_PROPERTIES_DIALOG_TXT);
    set_current_flow_box_config_to_dialog(flow_box_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_frame_config(View *target_view)
{
    FrameConfig *frame_config = read_frame_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, FRAME_PROPERTIES_DIALOG_TXT);
    set_current_frame_config_to_dialog(frame_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_grid_config(View *target_view)
{
    GridConfig *grid_config = read_grid_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, GRID_PROPERTIES_DIALOG_TXT);
    set_current_grid_config_to_dialog(grid_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_notebook_config(View *target_view)
{
    NotebookConfig *notebook_config = read_notebook_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, NOTEBOOK_PROPERTIES_DIALOG_TXT);
    set_current_notebook_config_to_dialog(notebook_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_overlay_config(View *target_view)
{
    OverlayConfig *overlay_config = read_overlay_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, OVERLAY_PROPERTIES_DIALOG_TXT);
    set_current_overlay_config_to_dialog(overlay_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_paned_config(View *target_view)
{
    PanedConfig *paned_config = read_paned_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, PANED_PROPERTIES_DIALOG_TXT);
    set_current_paned_config_to_dialog(paned_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_stack_config(View *target_view)
{
    StackConfig *stack_config = read_stack_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, STACK_PROPERTIES_DIALOG_TXT);
    set_current_stack_config_to_dialog(stack_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

// simple widgets
GtkWidget *prepare_update_button_config(View *target_view)
{
    ButtonConfig *button_config = read_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_button_config_to_dialog(button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_check_button_config(View *target_view)
{
    CheckButtonConfig *check_button_config = read_check_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, CHECK_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_check_button_config_to_dialog(check_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_color_button_config(View *target_view)
{
    ColorButtonConfig *color_button_config = read_color_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, COLOR_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_color_button_config_to_dialog(color_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_link_button_config(View *target_view)
{
    LinkButtonConfig *link_button_config = read_link_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, LINK_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_link_button_config_to_dialog(link_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_radio_button_config(View *target_view)
{
    RadioButtonConfig *radio_button_config = read_radio_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, RADIO_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_radio_button_config_to_dialog(radio_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_spin_button_config(View *target_view)
{
    SpinButtonConfig *spin_button_config = read_spin_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, SPIN_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_spin_button_config_to_dialog(spin_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_switch_button_config(View *target_view)
{
    SwitchButtonConfig *switch_button_config = read_switch_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, SWITCH_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_switch_button_config_to_dialog(switch_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_toggle_button_config(View *target_view)
{
    ToggleButtonConfig *toggle_button_config = read_toggle_button_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, TOGGLE_BUTTON_PROPERTIES_DIALOG_TXT);
    set_current_toggle_button_config_to_dialog(toggle_button_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_menu_bar_config(View *target_view)
{
    MenuBarConfig *menu_bar_config = read_menu_bar_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, MENU_BAR_PROPERTIES_DIALOG_TXT);
    set_current_menu_bar_config_to_dialog(menu_bar_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_menu_item_config(View *target_view)
{
    MenuItemConfig *menu_item_config = read_menu_item_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, MENU_ITEM_PROPERTIES_DIALOG_TXT);
    set_current_menu_item_config_to_dialog(menu_item_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_menu_config(View *target_view)
{
    MenuConfig *menu_config = read_menu_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, MENU_PROPERTIES_DIALOG_TXT);
    set_current_menu_config_to_dialog(menu_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_entry_config(View *target_view)
{
    EntryConfig *entry_config = read_entry_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, ENTRY_PROPERTIES_DIALOG_TXT);
    set_current_entry_config_to_dialog(entry_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_image_config(View *target_view)
{
    ImageConfig *image_config = read_image_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, IMAGE_PROPERTIES_DIALOG_TXT);
    set_current_image_config_to_dialog(image_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_label_config(View *target_view)
{
    LabelConfig *label_config = read_label_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, LABEL_PROPERTIES_DIALOG_TXT);
    set_current_label_config_to_dialog(label_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_progress_bar_config(View *target_view)
{
    ProgressBarConfig *progress_bar_config = read_progress_bar_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, PROGRESS_BAR_PROPERTIES_DIALOG_TXT);
    set_current_progress_bar_config_to_dialog(progress_bar_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_separator_config(View *target_view)
{
    SeparatorConfig *separator_config = read_separator_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, SEPARATOR_PROPERTIES_DIALOG_TXT);
    set_current_separator_config_to_dialog(separator_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_text_area_config(View *target_view)
{
    TextAreaConfig *text_area_config = read_text_area_config_from_widget(target_view->widget);
    root_dialog_view_global = build_app(root_app, NULL, TEXT_AREA_PROPERTIES_DIALOG_TXT);
    set_current_text_area_config_to_dialog(text_area_config);
    set_current_view_config_to_dialog(target_view->view_config);
    return root_dialog_view_global->widget;
}

GtkWidget *prepare_update_combo_text_box_config(View *target_view)
{
    // ComboTextBoxConfig *combo_text_box_config = read_combo_text_box_config_from_widget(target_view->widget);
    // root_dialog_view_global = build_app(root_app, NULL, COMBO_TEXT_BOX_PROPERTIES_DIALOG_TXT);
    // set_current_combo_text_box_config_to_dialog(combo_text_box_config);
    // set_current_view_config_to_dialog(target_view->view_config);
    // return root_dialog_view_global->widget;
    return NULL;
}

// GtkWidget *prepare_update_event_box_config(View *target_view)
// {
//     EventBoxConfig *event_box_config = read_event_box_config_from_widget(target_view->widget);
//     build_app(root_app, NULL, EVENT_BOX_PROPERTIES_DIALOG_TXT);
//     set_current_event_box_config_to_dialog(event_box_config);
//     set_current_view_config_to_dialog(target_view->view_config);
//     return root_dialog_view_global->widget;
// }

// GtkWidget *prepare_update_expander_config(View *target_view)
// {
//     ExpanderConfig *expander_config = read_expander_config_from_widget(target_view->widget);
//     build_app(root_app, NULL, EXPANDER_PROPERTIES_DIALOG_TXT);
//     set_current_expander_config_to_dialog(expander_config);
//     set_current_view_config_to_dialog(target_view->view_config);
//     return root_dialog_view_global->widget;
// }

// GtkWidget *prepare_update_list_box_config(View *target_view)
// {
//     ListBoxConfig *list_box_config = read_list_box_config_from_widget(target_view->widget);
//     build_app(root_app, NULL, LIST_BOX_PROPERTIES_DIALOG_TXT);
//     set_current_list_box_config_to_dialog(list_box_config);
//     set_current_view_config_to_dialog(target_view->view_config);
//     return root_dialog_view_global->widget;
// }

// update config
// containers
View *update_scrolled_window_config()
{
    g_print("UPDATING BOX WIDGET\n");

    ScrolledWindowConfig *scrolled_window_config = read_scrolled_window_config_from_dialog();
    g_print("1");

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);
    g_print("2");
    GtkWidget *scrolled_window_widget = create_scrolled_window(*scrolled_window_config);
    g_print("3");
    View *updated_scrolled_window_view = create_view(scrolled_window_widget, view_config);
    g_print("4");
    return apply_update(updated_scrolled_window_view, view_config);
}

View *update_box_config()
{
    g_print("UPDATING BOX WIDGET\n");

    BoxConfig *box_config = read_box_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *box_widget = create_box(*box_config);

    View *updated_box_view = create_view(box_widget, view_config);

    return apply_update(updated_box_view, view_config);
}

View *update_fixed_config()
{
    g_print("UPDATING FIXED WIDGET\n");

    FixedConfig *fixed_config = read_fixed_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *fixed_widget = create_fixed(*fixed_config);

    View *updated_fixed_view = create_view(fixed_widget, view_config);

    return apply_update(updated_fixed_view, view_config);
}

View *update_flow_box_config()
{
    g_print("UPDATING FLOW BOX WIDGET\n");

    FlowBoxConfig *flow_box_config = read_flow_box_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *flow_box_widget = create_flow_box(*flow_box_config);

    View *updated_flow_box_view = create_view(flow_box_widget, view_config);

    return apply_update(updated_flow_box_view, view_config);
}

View *update_frame_config()
{
    g_print("UPDATING FRAME WIDGET\n");

    FrameConfig *frame_config = read_frame_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *frame_widget = create_frame(*frame_config);

    View *updated_frame_view = create_view(frame_widget, view_config);

    return apply_update(updated_frame_view, view_config);
}

View *update_grid_config()
{
    g_print("UPDATING GRID WIDGET\n");

    GridConfig *grid_config = read_grid_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *grid_widget = create_grid(*grid_config);

    View *updated_grid_view = create_view(grid_widget, view_config);

    return apply_update(updated_grid_view, view_config);
}

View *update_notebook_config()
{
    g_print("UPDATING NOTEBOOK WIDGET\n");

    NotebookConfig *notebook_config = read_notebook_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *notebook_widget = create_notebook(*notebook_config);

    View *updated_notebook_view = create_view(notebook_widget, view_config);

    return apply_update(updated_notebook_view, view_config);
}

View *update_overlay_config()
{
    g_print("UPDATING OVERLAY WIDGET\n");

    OverlayConfig *overlay_config = read_overlay_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *overlay_widget = create_overlay(*overlay_config);

    View *updated_overlay_view = create_view(overlay_widget, view_config);

    return apply_update(updated_overlay_view, view_config);
}

View *update_paned_config()
{
    g_print("UPDATING PANED WIDGET\n");

    PanedConfig *paned_config = read_paned_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *paned_widget = create_paned(*paned_config);

    View *updated_paned_view = create_view(paned_widget, view_config);

    return apply_update(updated_paned_view, view_config);
}

View *update_stack_config()
{
    g_print("UPDATING STACK WIDGET\n");

    StackConfig *stack_config = read_stack_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *stack_widget = create_stack(*stack_config);

    View *updated_stack_view = create_view(stack_widget, view_config);

    return apply_update(updated_stack_view, view_config);
}

// simple widgets
View *update_button_config()
{
    g_print("UPDATING BUTTON WIDGET\n");

    ButtonConfig *button_config = read_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *button_widget = create_button(*button_config);

    View *updated_button_view = create_view(button_widget, view_config);

    return apply_update(updated_button_view, view_config);
}

View *update_check_button_config()
{
    g_print("UPDATING CHECK BUTTON WIDGET\n");

    CheckButtonConfig *check_button_config = read_check_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *check_button_widget = create_check_button(*check_button_config);

    View *updated_check_button_view = create_view(check_button_widget, view_config);

    return apply_update(updated_check_button_view, view_config);
}

View *update_color_button_config()
{
    g_print("UPDATING COLOR BUTTON WIDGET\n");

    ColorButtonConfig *color_button_config = read_color_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *color_button_widget = create_color_button(*color_button_config);

    View *updated_color_button_view = create_view(color_button_widget, view_config);

    return apply_update(updated_color_button_view, view_config);
}

View *update_link_button_config()
{
    g_print("UPDATING LINK BUTTON WIDGET\n");

    LinkButtonConfig *link_button_config = read_link_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *link_button_widget = create_link_button(*link_button_config);

    View *updated_link_button_view = create_view(link_button_widget, view_config);

    return apply_update(updated_link_button_view, view_config);
}

View *update_radio_button_config()
{
    g_print("UPDATING RADIO BUTTON WIDGET\n");

    RadioButtonConfig *radio_button_config = read_radio_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *radio_button_widget = create_radio_button(*radio_button_config);

    View *updated_radio_button_view = create_view(radio_button_widget, view_config);

    return apply_update(updated_radio_button_view, view_config);
}

View *update_spin_button_config()
{
    g_print("UPDATING SPIN BUTTON WIDGET\n");

    SpinButtonConfig *spin_button_config = read_spin_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *spin_button_widget = create_spin_button(*spin_button_config);

    View *updated_spin_button_view = create_view(spin_button_widget, view_config);

    return apply_update(updated_spin_button_view, view_config);
}

View *update_switch_button_config()
{
    g_print("UPDATING SWITCH BUTTON WIDGET\n");

    SwitchButtonConfig *switch_button_config = read_switch_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *switch_button_widget = create_switch_button(*switch_button_config);

    View *updated_switch_button_view = create_view(switch_button_widget, view_config);

    return apply_update(updated_switch_button_view, view_config);
}

View *update_toggle_button_config()
{
    g_print("UPDATING TOGGLE BUTTON WIDGET\n");

    ToggleButtonConfig *toggle_button_config = read_toggle_button_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *toggle_button_widget = create_toggle_button(*toggle_button_config);

    View *updated_toggle_button_view = create_view(toggle_button_widget, view_config);

    return apply_update(updated_toggle_button_view, view_config);
}

View *update_menu_bar_config()
{
    g_print("UPDATING MENU BAR WIDGET\n");

    MenuBarConfig *menu_bar_config = read_menu_bar_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *menu_bar_widget = create_menu_bar(*menu_bar_config);

    View *updated_menu_bar_view = create_view(menu_bar_widget, view_config);

    return apply_update(updated_menu_bar_view, view_config);
}

View *update_menu_item_config()
{
    g_print("UPDATING MENU ITEM WIDGET\n");

    MenuItemConfig *menu_item_config = read_menu_item_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *menu_item_widget = create_menu_item(*menu_item_config);

    View *updated_menu_item_view = create_view(menu_item_widget, view_config);

    return apply_update(updated_menu_item_view, view_config);
}

View *update_menu_config()
{
    g_print("UPDATING MENU WIDGET\n");

    MenuConfig *menu_config = read_menu_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *menu_widget = create_menu(*menu_config);

    View *updated_menu_view = create_view(menu_widget, view_config);

    return apply_update(updated_menu_view, view_config);
}

View *update_entry_config()
{
    g_print("UPDATING ENTRY WIDGET\n");

    EntryConfig *entry_config = read_entry_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *entry_widget = create_entry(*entry_config);

    View *updated_entry_view = create_view(entry_widget, view_config);

    return apply_update(updated_entry_view, view_config);
}

View *update_image_config()
{
    g_print("UPDATING IMAGE WIDGET\n");

    ImageConfig *image_config = read_image_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *image_widget = create_image(*image_config);

    View *updated_image_view = create_view(image_widget, view_config);

    return apply_update(updated_image_view, view_config);
}

View *update_label_config()
{
    g_print("UPDATING LABEL WIDGET\n");

    LabelConfig *label_config = read_label_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *label_widget = create_label(*label_config);

    View *updated_label_view = create_view(label_widget, view_config);

    return apply_update(updated_label_view, view_config);
}

View *update_progress_bar_config()
{
    g_print("UPDATING PROGRESS BAR WIDGET\n");

    ProgressBarConfig *progress_bar_config = read_progress_bar_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *progress_bar_widget = create_progress_bar(*progress_bar_config);

    View *updated_progress_bar_view = create_view(progress_bar_widget, view_config);

    return apply_update(updated_progress_bar_view, view_config);
}

View *update_separator_config()
{
    g_print("UPDATING SEPARATOR WIDGET\n");

    SeparatorConfig *separator_config = read_separator_config_from_dialog();

    ViewConfig *view_config = read_view_config_from_dialog(TRUE);

    GtkWidget *separator_widget = create_separator(*separator_config);

    View *updated_separator_view = create_view(separator_widget, view_config);

    return apply_update(updated_separator_view, view_config);
}

// View *update_text_area_config()
// {
//     g_print("UPDATING TEXT AREA WIDGET\n");

//     TextAreaConfig *text_area_config = read_text_area_config_from_dialog();

//     ViewConfig *view_config = read_view_config_from_dialog(TRUE);

//     GtkWidget *text_area_widget = create_text_area(*text_area_config);

//     View *updated_text_area_view = create_view(text_area_widget, view_config);

//     return apply_update(updated_text_area_view, view_config);
// }

// View *update_combo_text_box_config()
// {
//     g_print("UPDATING COMBO TEXT BOX WIDGET\n");

//     ComboTextBoxConfig *combo_text_box_config = read_combo_text_box_config_from_dialog();

//     ViewConfig *view_config = read_view_config_from_dialog(TRUE);

//     GtkWidget *combo_text_box_widget = create_combo_text_box(*combo_text_box_config);

//     View *updated_combo_text_box_view = create_view(combo_text_box_widget, view_config);

//     return apply_update(updated_combo_text_box_view, view_config);
// }

// View *update_event_box_config()
// {
//     g_print("UPDATING EVENT BOX WIDGET\n");

//     EventBoxConfig *event_box_config = read_event_box_config_from_dialog();

//     ViewConfig *view_config = read_view_config_from_dialog(TRUE);

//     GtkWidget *event_box_widget = create_event_box(*event_box_config);

//     View *updated_event_box_view = create_view(event_box_widget, view_config);

//     return apply_update(updated_event_box_view, view_config);
// }

// View *update_expander_config()
// {
//     g_print("UPDATING EXPANDER WIDGET\n");

//     ExpanderConfig *expander_config = read_expander_config_from_dialog();

//     ViewConfig *view_config = read_view_config_from_dialog(TRUE);

//     GtkWidget *expander_widget = create_expander(*expander_config);

//     View *updated_expander_view = create_view(expander_widget, view_config);

//     return apply_update(updated_expander_view, view_config);
// }

// View *update_list_box_config()
// {
//     g_print("UPDATING LIST BOX WIDGET\n");

//     ListBoxConfig *list_box_config = read_list_box_config_from_dialog();

//     ViewConfig *view_config = read_view_config_from_dialog(TRUE);

//     GtkWidget *list_box_widget = create_list_box(*list_box_config);

//     View *updated_list_box_view = create_view(list_box_widget, view_config);

//     return apply_update(updated_list_box_view, view_config);
// }
