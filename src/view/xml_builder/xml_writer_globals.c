#include "./../../../include/widgets/view/xml_builder/xml_writer_globals.h"

void print_tabs(FILE *output_file, int tabs_number)
{
    for (int i = 0; i < tabs_number; fprintf(output_file, "\t", i++))
        ;
}

void write_widget_style(FILE *output_file, GtkWidget *widget, int tabs_number)
{
    if (!output_file || !widget)
        return;

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    GdkRGBA fg_rgba, bg_rgba;

    // Get the widget's foreground color (for the normal state)
    gtk_style_context_get_color(context, GTK_STATE_FLAG_NORMAL, &fg_rgba);
    guint fg_r = (guint)(fg_rgba.red * 255);
    guint fg_g = (guint)(fg_rgba.green * 255);
    guint fg_b = (guint)(fg_rgba.blue * 255);
    gchar fg_hex[8]; // Format: "#RRGGBB"
    sprintf(fg_hex, "#%02X%02X%02X", fg_r, fg_g, fg_b);

    // Create a temporary surface to render the background
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
    cairo_t *cr = cairo_create(surface);

    // Render the background
    gtk_render_background(context, cr, 0, 0, 1, 1);

    // Get the pixel color from the surface
    unsigned char *data = cairo_image_surface_get_data(surface);
    guint bg_r = data[2];
    guint bg_g = data[1];
    guint bg_b = data[0];
    gchar bg_hex[8] = "\0";

    if (bg_r || bg_g || bg_b) // Check if the background color is not empty
    {
        sprintf(bg_hex, "#%02X%02X%02X", bg_r, bg_g, bg_b);
    }

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    // Get widget dimensions from the allocation.
    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);

    // Retrieve widget margins.
    gint margin_top = gtk_widget_get_margin_top(widget);
    gint margin_bottom = gtk_widget_get_margin_bottom(widget);
    gint margin_left = gtk_widget_get_margin_start(widget);
    gint margin_right = gtk_widget_get_margin_end(widget);

    // Retrieve font details using Pango.
    PangoContext *pcontext = gtk_widget_get_pango_context(widget);
    PangoFontDescription *font_desc = pango_context_get_font_description(pcontext);
    const char *family = font_desc ? pango_font_description_get_family(font_desc) : "default";
    int font_size = font_desc ? (pango_font_description_get_size(font_desc) / PANGO_SCALE) : 0;

    // Get font style (Normal, Italic, Oblique)
    const char *font_style = "Normal";
    if (font_desc)
    {
        PangoStyle style = pango_font_description_get_style(font_desc);
        if (style == PANGO_STYLE_ITALIC)
            font_style = "Italic";
        else if (style == PANGO_STYLE_OBLIQUE)
            font_style = "Oblique";
    }

    // Get font weight (e.g., Bold, Normal)
    const char *font_weight = "Normal";
    if (font_desc)
    {
        PangoWeight weight = pango_font_description_get_weight(font_desc);
        if (weight >= PANGO_WEIGHT_BOLD)
            font_weight = "Bold";
    }

    // Write all properties to the output file with the requested tabs.
    if (g_strcmp0(fg_hex, "#2E3436")) // Checking if the foreground color is not the default one
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "color=\"%s\"\n", fg_hex);
    }

    if (strlen(bg_hex) && g_strcmp0(bg_hex, "B50000") != 0 && g_strcmp0(bg_hex, "\0")) // Check if the background color was found
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "bg_color=\"%s\"\n", bg_hex);
    }

    if (width != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "width=\"%d\"\n", width);
    }
    if (height != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "height=\"%d\"\n", height);
    }

    if (margin_top)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "margin_top=\"%d\"\n", margin_top);
    }
    if (margin_bottom)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "margin_bottom=\"%d\"\n", margin_bottom);
    }
    if (margin_left)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "margin_left=\"%d\"\n", margin_left);
    }
    if (margin_right)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "margin_right=\"%d\"\n", margin_right);
    }

    if (font_size != 9) // Checking if font size is the default
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "font_size=\"%d\"\n", font_size);
    }

    if (g_strcmp0(family, "Segoe UI")) // Checking if font family is not default "Segoe UI"
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "font_family=\"%s\"\n", family);
    }

    // Optionally, you can add font style and weight as well:
    if (g_strcmp0(font_style, "Normal"))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "font_style=\"%s\"\n", font_style);
    }

    if (g_strcmp0(font_weight, "Normal"))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "font_weight=\"%s\"\n", font_weight);
    }

    gchar *bg_image = g_object_get_data(G_OBJECT(widget), "bg_image");

    if (bg_image && g_strcmp0(bg_image, "\0"))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "bg_image=\"%s\"\n", bg_image);
    }
}

void write_widget_view_config(FILE *output_file, View *view, int tabs_number)
{
    if (view->view_config->position_x)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "position_x=\"%d\"\n", view->view_config->position_x);
    }
    if (view->view_config->position_y)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "position_y=\"%d\"\n", view->view_config->position_y);
    }
    if (view->view_config->pack_direction != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "pack_direction=\"%d\"\n", view->view_config->pack_direction);
    }

    if (view->view_config->box_expand)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "box_expand=\"true\"\n");
    }
    if (view->view_config->box_fill)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "box_fill=\"true\"\n");
    }
    if (view->view_config->box_padding)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "box_padding=\"%d\"\n", view->view_config->box_padding);
    }
    if (view->view_config->flow_box_order)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "flow_box_order=\"%d\"\n", view->view_config->flow_box_order);
    }
    if (view->view_config->paned_order)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "paned_order=\"%d\"\n", view->view_config->paned_order);
    }
    if (g_strcmp0(view->view_config->tab_label, "\0"))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "tab_label=\"%s\"\n", view->view_config->tab_label);
    }
    if (!view->view_config->is_reorderable)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "is_reorderable=\"false\"\n");
    }
    if (view->view_config->row)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "row=\"%d\"\n", view->view_config->row);
    }
    if (view->view_config->column)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "column=\"%d\"\n", view->view_config->column);
    }
    if (view->view_config->row_span != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "row_span=\"%d\"\n", view->view_config->row_span);
    }
    if (view->view_config->column_span != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "column_span=\"%d\"\n", view->view_config->column_span);
    }
    if (g_strcmp0(view->view_config->menu_orientation, "vertical"))
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "menu_orientation=\"%s\"\n", view->view_config->menu_orientation);
    }

    if (view->view_config->menu_top)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "menu_top=\"%d\"\n", view->view_config->menu_top);
    }
    if (view->view_config->menu_bottom != 1)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "menu_bottom=\"%d\"\n", view->view_config->menu_bottom);
    }
    if (view->view_config->menu_left)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "menu_left=\"%d\"\n", view->view_config->menu_left);
    }
    if (view->view_config->menu_right)
    {
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "menu_right=\"%d\"\n", view->view_config->menu_right);
    }
}

void write_widget_signale(FILE *output_file, View *view, int tabs_number)
{
    // check if there is a signal to add
    if (view->view_config->signal.event_type == SIG_NONE || view->view_config->signal.event_type == SIG_LAST)
        return;

    switch (view->view_config->signal.event_type)
    {
    case SIG_ON_CLICK:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_click=");
        break;
    case SIG_ON_TOGGLE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_toggle=");
        break;
    case SIG_ON_CHANGE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_change=");
        break;
    case SIG_ON_ACTIVATE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_activate=");
        break;
    case SIG_ON_DELETE_EVENT:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_delete_event=");
        break;
    case SIG_ON_DESTROY:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_destroy=");
        break;
    case SIG_ON_VALUE_CHANGED:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_value_changed=");
        break;
    case SIG_ON_ROW_ACTIVATED:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_row_activated=");
        break;
    case SIG_ON_KEY_PRESS:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_key_press=");
        break;
    case SIG_ON_KEY_RELEASE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_key_release=");
        break;
    case SIG_ON_BUTTON_PRESS:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_button_press=");
        break;
    case SIG_ON_BUTTON_RELEASE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_button_release=");
        break;
    case SIG_ON_MOTION_NOTIFY:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_motion_notify=");
        break;
    case SIG_ON_FOCUS_IN:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_focus_in=");
        break;
    case SIG_ON_FOCUS_OUT:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_focus_out=");
        break;
    case SIG_ON_SWITCH_PAGE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_switch_page=");
        break;
    case SIG_ON_SELECTION_CHANGED:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_selection_changed=");
        break;
    case SIG_ON_POPUP_MENU:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_popup_menu=");
        break;
    case SIG_ON_SCROLL:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_scroll=");
        break;
    case SIG_ON_ENTER_NOTIFY:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_enter_notify=");
        break;
    case SIG_ON_LEAVE_NOTIFY:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_leave_notify=");
        break;
    case SIG_ON_RESPONSE:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_response=");
        break;
    case SIG_ON_COLOR_SET:
        print_tabs(output_file, tabs_number);
        fprintf(output_file, "on_color_set=");
        break;
    default:
        // No action for SIG_NONE, SIG_LAST, or unknown signals
        return;
        break;
    }

    fprintf(output_file, "\"%s\"\n", view->view_config->signal.sig_handler);
}
void write_widget_tag_style_view_config(FILE *output_file, View *view, gchar *tag, int tabs_number)
{
    // print the tag
    print_tabs(output_file, tabs_number);
    fprintf(output_file, "<%s \n", tag);

    // print the id
    print_tabs(output_file, tabs_number + 1);
    fprintf(output_file, "id=\"%s\"\n", view->view_config->view_id);

    // print the style
    write_widget_style(output_file, view->widget, tabs_number + 1);

    // print the view config
    // todo finish the view config
    write_widget_view_config(output_file, view, tabs_number + 1);
    write_widget_signale(output_file, view, tabs_number + 1);
}