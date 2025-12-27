#ifndef WIDGET_UPDATE_H
#define WIDGET_UPDATE_H

#include "./../../builder.h"

extern View *temp_parent_view;

void rebuild_graph(View **temp_root, View *view, View *updated_view, gchar *view_id, gboolean relative_container);

void destroy_dialog_input(gchar *view_id);

void disable_dialog_input(gchar *view_id);

View *apply_update(View *updated_view, ViewConfig *view_config);

void set_current_view_config_to_dialog(ViewConfig *view_config);

// set current config to dialog
// containers
void set_current_scrolled_window_config_to_dialog(ScrolledWindowConfig *scrolled_window_config);

void set_current_box_config_to_dialog(BoxConfig *box_config);

void set_current_fixed_config_to_dialog(FixedConfig *fixed_config);

void set_current_flow_box_config_to_dialog(FlowBoxConfig *flow_box_config);

void set_current_frame_config_to_dialog(FrameConfig *frame_config);

void set_current_grid_config_to_dialog(GridConfig *grid_config);

void set_current_notebook_config_to_dialog(NotebookConfig *notebook_config);

void set_current_overlay_config_to_dialog(OverlayConfig *overlay_config);

void set_current_paned_config_to_dialog(PanedConfig *paned_config);

void set_current_stack_config_to_dialog(StackConfig *stack_config);

// simple widgets
void set_current_button_config_to_dialog(ButtonConfig *button_config);

void set_current_check_button_config_to_dialog(CheckButtonConfig *check_button_config);

void set_current_color_button_config_to_dialog(ColorButtonConfig *color_button_config);

void set_current_link_button_config_to_dialog(LinkButtonConfig *link_config);

void set_current_radio_button_config_to_dialog(RadioButtonConfig *radio_button_config);

void set_current_spin_button_config_to_dialog(SpinButtonConfig *spin_button_config);

void set_current_switch_button_config_to_dialog(SwitchButtonConfig *switch_button_config);

void set_current_toggle_button_config_to_dialog(ToggleButtonConfig *toggle_button_config);

void set_current_menu_bar_config_to_dialog(MenuBarConfig *menu_bar_config);

void set_current_menu_item_config_to_dialog(MenuItemConfig *menu_item_config);

void set_current_menu_config_to_dialog(MenuConfig *menu_config);

void set_current_entry_config_to_dialog(EntryConfig *entry_config);

void set_current_image_config_to_dialog(ImageConfig *image_config);

void set_current_label_config_to_dialog(LabelConfig *label_config);

void set_current_progress_bar_config_to_dialog(ProgressBarConfig *progress_bar_config);

void set_current_separator_config_to_dialog(SeparatorConfig *separator_config);

void set_current_text_area_config_to_dialog(TextAreaConfig *text_area_config);

void set_current_combo_text_box_config_to_dialog(ComboTextBoxConfig *combo_text_box_config);

// prepare the update dialog and show it
// containers

GtkWidget *prepare_update_scrolled_window_config(View *target_view);

GtkWidget *prepare_update_box_config(View *target_view);

GtkWidget *prepare_update_fixed_config(View *target_view);

GtkWidget *prepare_update_flow_box_config(View *target_view);

GtkWidget *prepare_update_frame_config(View *target_view);

GtkWidget *prepare_update_grid_config(View *target_view);

GtkWidget *prepare_update_notebook_config(View *target_view);

GtkWidget *prepare_update_overlay_config(View *target_view);

GtkWidget *prepare_update_paned_config(View *target_view);

GtkWidget *prepare_update_stack_config(View *target_view);

// simple widgets
GtkWidget *prepare_update_button_config(View *target_view);

GtkWidget *prepare_update_check_button_config(View *target_view);

GtkWidget *prepare_update_color_button_config(View *target_view);

GtkWidget *prepare_update_link_button_config(View *target_view);

GtkWidget *prepare_update_radio_button_config(View *target_view);

GtkWidget *prepare_update_spin_button_config(View *target_view);

GtkWidget *prepare_update_switch_button_config(View *target_view);

GtkWidget *prepare_update_toggle_button_config(View *target_view);

GtkWidget *prepare_update_menu_bar_config(View *target_view);

GtkWidget *prepare_update_menu_item_config(View *target_view);

GtkWidget *prepare_update_menu_config(View *target_view);

GtkWidget *prepare_update_entry_config(View *target_view);

GtkWidget *prepare_update_image_config(View *target_view);

GtkWidget *prepare_update_label_config(View *target_view);

GtkWidget *prepare_update_progress_bar_config(View *target_view);

GtkWidget *prepare_update_separator_config(View *target_view);

GtkWidget *prepare_update_text_area_config(View *target_view);

GtkWidget *prepare_update_combo_text_box_config(View *target_view);

// update config
// containers
View *update_scrolled_window_config();

View *update_box_config();

View *update_fixed_config();

View *update_flow_box_config();

View *update_frame_config();

View *update_grid_config();

View *update_notebook_config();

View *update_overlay_config();

View *update_paned_config();

View *update_stack_config();

// simple widgets
View *update_button_config();

View *update_check_button_config();

View *update_color_button_config();

View *update_link_button_config();

View *update_radio_button_config();

View *update_spin_button_config();

View *update_switch_button_config();

View *update_toggle_button_config();

View *update_menu_bar_config();

View *update_menu_item_config();

View *update_menu_config();

View *update_entry_config();

View *update_image_config();

View *update_label_config();

View *update_progress_bar_config();

View *update_separator_config();

// View *update_text_area_config();

// View *update_combo_text_box_config();

#endif