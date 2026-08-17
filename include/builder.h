#ifndef BUILDER_H
#define BUILDER_H

#include "./index.h"
#define INDEX_TXT "./src/view/index.xml"
#define DIALOG_TXT "./src/view/dialog.xml"
#define MYDIALOG_TXT "./src/view/my_dialog.xml"
#define IMPORTDIALOG_TXT "./src/view/import_dialog.xml"
#define ERROR_DIALOG_TXT "./src/view/error_dialog.xml"
#define BOX_PROPERTIES_DIALOG_TXT "./src/view/properties/box_dialog.xml"
#define FIXED_PROPERTIES_DIALOG_TXT "./src/view/properties/fixed_dialog.xml"
#define GRID_PROPERTIES_DIALOG_TXT "./src/view/properties/grid_dialog.xml"
#define FLOW_BOX_PROPERTIES_DIALOG_TXT "./src/view/properties/flow_box_dialog.xml"
#define PANED_PROPERTIES_DIALOG_TXT "./src/view/properties/paned_dialog.xml"
#define FRAME_PROPERTIES_DIALOG_TXT "./src/view/properties/frame_dialog.xml"
#define OVERLAY_PROPERTIES_DIALOG_TXT "./src/view/properties/overlay_dialog.xml"
#define NOTEBOOK_PROPERTIES_DIALOG_TXT "./src/view/properties/notebook_dialog.xml"
#define BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/button_dialog.xml"
#define ENTRY_PROPERTIES_DIALOG_TXT "./src/view/properties/entry_dialog.xml"
#define SCROLLED_WINDOW_PROPERTIES_DIALOG_TXT "./src/view/properties/scrolled_window_dialog.xml"
#define STACK_PROPERTIES_DIALOG_TXT "./src/view/properties/stack_dialog.xml"
#define LABEL_PROPERTIES_DIALOG_TXT "./src/view/properties/label_dialog.xml"
#define SEPARATOR_PROPERTIES_DIALOG_TXT "./src/view/properties/separator_dialog.xml"
#define MENU_BAR_PROPERTIES_DIALOG_TXT "./src/view/properties/menu_bar_dialog.xml"
#define MENU_PROPERTIES_DIALOG_TXT "./src/view/properties/menu_dialog.xml"
#define MENU_ITEM_PROPERTIES_DIALOG_TXT "./src/view/properties/menu_item_dialog.xml"
#define RADIO_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/radio_button_dialog.xml"
#define SPIN_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/spin_button_dialog.xml"
// #define LIST_BOX_PROPERTIES_DIALOG_TXT "./src/view/properties/list_box_dialog.xml"
#define IMAGE_PROPERTIES_DIALOG_TXT "./src/view/properties/image_dialog.xml"
#define PROGRESS_BAR_PROPERTIES_DIALOG_TXT "./src/view/properties/progress_bar_dialog.xml"
#define CHECK_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/check_button_dialog.xml"
#define LINK_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/link_button_dialog.xml"
#define TEXT_AREA_PROPERTIES_DIALOG_TXT "./src/view/properties/text_area_dialog.xml"
#define COMBO_TEXT_BOX_PROPERTIES_DIALOG_TXT "./src/view/properties/combo_text_box_dialog.xml"
#define TOGGLE_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/toggle_button_dialog.xml"
#define COLOR_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/color_button_dialog.xml"
#define EXPANDER_PROPERTIES_DIALOG_TXT "./src/view/properties/expander_dialog.xml"
#define EVENT_BOX_PROPERTIES_DIALOG_TXT "./src/view/properties/event_box_dialog.xml"
#define SWITCH_BUTTON_PROPERTIES_DIALOG_TXT "./src/view/properties/switch_button_dialog.xml"

#define MODE "r"

typedef enum
{
    WindowTag,
    ScrolledWindowTag,
    BoxTag,
    FixedTag,
    StackTag,
    NotebookTag,
    ButtonTag,
    SwitchButtonTag,
    EntryTag,
    LabelTag,
    SeparatorTag,
    MenuBarTag,
    MenuTag,
    MenuItemTag,
    RadioButtonTag,
    SpinButtonTag,
    FlowBoxTag,
    ListBoxTag,
    GridTag,
    PanedTag,
    ImageTag,
    ProgressBarTag,
    CheckButtonTag,
    LinkButtonTag,
    FrameTag,
    TextAreaTag,
    OverlayTag,
    DialogTag,
    ComboTextBoxTag,
    ToggleButtonTag,
    ColorButtonTag,
    ExpanderTag,
    EventBoxTag
} Tags;

// Global structure;

View *create_view(GtkWidget *widget, ViewConfig *view_config);

View *add_view(View *view, View *relative, gboolean is_relative_container);

gchar *read_tag(FILE *index);

int get_view_index(gchar *widget_tag);

View *build_app(GtkApplication *app, View *root_view, const gchar *file_path);

void connect_signals(View *view);

void add_command(const gchar *label, gint x, gint y, const gchar *signal_option, View *commands_container, View *root_view);

void add_custom_command(ViewConfig *view_conf, const gchar *label, gint x, gint y, View *commands_container, View *root_view);

#endif