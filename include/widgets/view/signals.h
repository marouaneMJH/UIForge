#ifndef SIGNAL_H
#define SIGNAL_H

#include "./../../global.h"


/**
 * @brief Most commonly used GTK3 signals
 *
 * This enum defines the most frequently used signals in GTK3 applications.
 * It is used to identify which signals are connected to the current widget.
 * Each signal corresponds to a specific event, such as a button click, text change,
 * or window close request. These signals help in managing user interactions
 * and widget behavior within the application.
 *
 * Usage:
 * - Use this enum to reference signals in a more readable and organized way.
 * - Combine with a signal mapping function to dynamically connect signals.
 * - Used in SignalConfig to know whitch signale the widget use
 *
 * Example:
 * SignalType signal = SIG_ON_CLICK;
 * g_signal_connect(widget, get_signal_name(signal), G_CALLBACK(callback_function), NULL);
 */
typedef enum
{
    SIG_NONE, // No signal

    SIG_ON_CLICK,             // Triggered when a button is clicked.
    SIG_ON_TOGGLE,            // Triggered when a toggleable widget changes state.
    SIG_ON_CHANGE,            // Triggered when content changes.
    SIG_ON_ACTIVATE,          // Triggered when Enter is pressed inside a widget.
    SIG_ON_DELETE_EVENT,      // Triggered when a window is requested to close.
    SIG_ON_DESTROY,           // Triggered when a widget is destroyed.
    SIG_ON_VALUE_CHANGED,     // Triggered when a value changes (e.g., slider moves).
    SIG_ON_ROW_ACTIVATED,     // Triggered when a row is double-clicked or activated.
    SIG_ON_KEY_PRESS,         // Triggered when a key is pressed while widget has focus.
    SIG_ON_KEY_RELEASE,       // Triggered when a key is released.
    SIG_ON_BUTTON_PRESS,      // Triggered when a mouse button is pressed.
    SIG_ON_BUTTON_RELEASE,    // Triggered when a mouse button is released.
    SIG_ON_MOTION_NOTIFY,     // Triggered when the mouse moves over a widget.
    SIG_ON_FOCUS_IN,          // Triggered when a widget gains focus.
    SIG_ON_FOCUS_OUT,         // Triggered when a widget loses focus.
    SIG_ON_SWITCH_PAGE,       // Triggered when switching tabs in a notebook.
    SIG_ON_SELECTION_CHANGED, // Triggered when selection changes in views (e.g., GtkTreeView).
    SIG_ON_POPUP_MENU,        // Triggered when a context menu is requested (right-click).
    SIG_ON_SCROLL,            // Triggered when the scroll wheel is used.
    SIG_ON_ENTER_NOTIFY,      // Triggered when the mouse enters a widget's area.
    SIG_ON_LEAVE_NOTIFY,      // Triggered when the mouse leaves a widget's area.
    SIG_ON_RESPONSE,          // Triggered when one of dialog button triggered
    SIG_ON_COLOR_SET,         // Triggered when the the the GtkColorButton changed

    SIG_LAST // Placeholder for the last signal
} SignalType;

/**
 * @brief Signal and its handler configuration
 *
 * This struct defines a mapping between a GTK signal and its corresponding handler function.
 * It is useful for managing multiple signals in a structured way, making it easier to
 * dynamically connect signals to their respective widgets in a GTK application.
 *
 * Fields:
 * - `sig_handler`: A string representing the name of the handler function that will be
 *    called when the signal is emitted.
 * - `event_name`: An enumeration value of type `SignalType` that specifies the signal type.
 *
 * Example usage:
 * @code
 * - XML
 *      on_click="sig_function"
 * - C
 * SignalConfig config = {
 *     .sig_handler = "sig_function",
 *     .event_name = SIG_ON_CLICK
 * };

 * @endcode
 */
typedef struct
{
    gchar sig_handler[MAX_SIGNAL_NAME_SIZE];
    SignalType event_type;
} SignalConfig;


/**
 * @brief structure for handle signales parametres
 * //todo variable: checck if on_cllick signal activate or not and also for other signales
 */
typedef struct
{
    gchar params[PARAM_COUNT][MAX_SIGNAL_NAME_SIZE]; // First function parameter

} ParamNode;


/*** Start Color Button ***/
/**
 * @brief this signale used on button color to change friend bg color
 *
 * signale to change friend bg color by id, it will search about the id
 * then it will give it the curent buttonColor color, to seach about the widget
 * we use param_1 witch is the widget id,should start with xxx (xxx-widget) to
 * know that the main widget in the dialog
 *
 * @param[in] widget ColorButton
 * @param[in] gpointer pointer contain ParamNode items
 *
 * @return[ou] void
 */
static void sig_color_btn_friend_bg_color(GtkWidget *widget, gpointer data);

/**
 * @brief change friend widget color
 *
 * like the  `sig_color_btn_friend_bg_color` but instead of bg_color we work with color
 */
static void sig_color_btn_friend_color(GtkWidget *widget, gpointer data);

/*** Start Color Button ***/

void set_available_scopes(const gchar *widget_type);

gboolean check_relative_container(GtkWidget *widget);

static void sig_import_ui_from_xml(GtkWidget *widget, gpointer data); 

static void sig_refrech_crud_ui(GtkWidget *widget, gpointer data);

extern void sig_properties_dialog(GtkWidget *widget, gpointer data);

#endif
