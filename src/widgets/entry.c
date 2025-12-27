#include "../../include/widgets/entry.h"

ViewConfig *configure_entry_property(EntryConfig *entry_config, ViewConfig *view_config, gchar *property, gchar *value)
{
    if (!entry_config || !property || !value)
        return NULL;

    if (g_strcmp0(property, "text") == 0)
        strcpy(entry_config->text, value);

    if (g_strcmp0(property, "placeholder_text") == 0)
        strcpy(entry_config->placeholder_text, value);

    if (g_strcmp0(property, "is_visible") == 0)
        entry_config->is_visible = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        entry_config->margins.top = atoi(value);

    if (g_strcmp0(property, "margin_bottom") == 0)
        entry_config->margins.bottom = atoi(value);

    if (g_strcmp0(property, "margin_left") == 0)
        entry_config->margins.start = atoi(value);

    if (g_strcmp0(property, "margin_right") == 0)
        entry_config->margins.end = atoi(value);

    // Dimensions
    if (g_strcmp0(property, "width") == 0)
        entry_config->dimensions.width = atoi(value);

    if (g_strcmp0(property, "height") == 0)
        entry_config->dimensions.height = atoi(value);

    // Appearance
    if (g_strcmp0(property, "bg_color") == 0)
        strcpy(entry_config->bg_color, value);

    if (g_strcmp0(property, "text_color") == 0)
        strcpy(entry_config->text_color, value);

    if (g_strcmp0(property, "opacity") == 0)
        entry_config->opacity = atof(value);

    if (g_strcmp0(property, "has_frame") == 0)
        entry_config->has_frame = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    if (g_strcmp0(property, "has_delete_icon") == 0)
        entry_config->has_delete_icon = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    // Text handling
    if (g_strcmp0(property, "overwrite_mode") == 0)
        entry_config->overwrite_mode = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    if (g_strcmp0(property, "max_length") == 0)
        entry_config->max_length = atoi(value);

    if (g_strcmp0(property, "alignment") == 0)
        entry_config->alignment = atof(value);

    if (g_strcmp0(property, "progress_fraction") == 0)
        entry_config->progress_fraction = atof(value);
    else if (g_strcmp0(property, "progress_pulse_step") == 0)
        entry_config->progress_pulse_step = atof(value);

    if (g_strcmp0(property, "activates_default") == 0)
        entry_config->activates_default = g_strcmp0(value, "true") == 0 ? TRUE : FALSE;

    if (g_strcmp0(property, "type") == 0)
    {
        if (g_strcmp0(value, "number") == 0)
            entry_config->purpose = GTK_INPUT_PURPOSE_NUMBER;
        else if (g_strcmp0(value, "email") == 0)
            entry_config->purpose = GTK_INPUT_PURPOSE_EMAIL;
        else if (g_strcmp0(value, "name") == 0)
            entry_config->purpose = GTK_INPUT_PURPOSE_NAME;
        else if (g_strcmp0(value, "password") == 0)
            entry_config->purpose = GTK_INPUT_PURPOSE_PASSWORD;
        else if (g_strcmp0(value, "pin") == 0)
            entry_config->purpose = GTK_INPUT_PURPOSE_PIN;
    }

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            entry_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            entry_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            entry_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            entry_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            entry_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            entry_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            entry_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            entry_config->halign = GTK_ALIGN_FILL;
    }

    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_entry_config(FILE *index, EntryConfig *entry_config)
{
    return init_generic_config(index, (void *)entry_config, (ConfigurePropertyCallback)configure_entry_property);
}

void numeric_field_handler(GtkEntry *entry, const gchar *text, gint length)
{
    // Allow only digits
    for (int i = 0; i < length; i++)
    {
        if (!g_ascii_isdigit(text[i]))
        {
            g_signal_stop_emission_by_name(entry, "insert-text");
            return;
        }
    }
}

void email_field_handler(GtkEntry *entry, const gchar *text, gint length)
{
    // Allow only email characters
    for (int i = 0; i < length; i++)
    {
        if (!g_ascii_isalnum(text[i]) && text[i] != '@' && text[i] != '.' && text[i] != '_')
        {
            g_signal_stop_emission_by_name(entry, "insert-text");
            return;
        }
    }
}

void on_icon_press(GtkEntry *entry, gpointer user_data)
{

    gtk_entry_set_text(entry, "");
}

GtkWidget *create_entry(EntryConfig entry_config)
{

    // g_print("=============================================\n");
    GtkWidget *entry = gtk_entry_new();

    if (entry_config.has_delete_icon)
    {
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(entry), GTK_ENTRY_ICON_SECONDARY, "edit-clear");
        const gchar *icon = gtk_entry_get_icon_name(GTK_ENTRY(entry), GTK_ENTRY_ICON_SECONDARY);
        if (icon)
            g_signal_connect(entry, "icon-press", G_CALLBACK(on_icon_press), NULL);
    }

    gtk_entry_set_text(GTK_ENTRY(entry), entry_config.text);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), entry_config.placeholder_text);
    gtk_entry_set_visibility(GTK_ENTRY(entry), entry_config.is_visible);
    gtk_entry_set_input_purpose(GTK_ENTRY(entry), entry_config.purpose);
    //
    if (entry_config.purpose == GTK_INPUT_PURPOSE_EMAIL)
        g_signal_connect(entry, "insert-text", G_CALLBACK(email_field_handler), NULL);

    if ((entry_config.purpose == GTK_INPUT_PURPOSE_NUMBER) || (entry_config.purpose == GTK_INPUT_PURPOSE_PIN))
        g_signal_connect(entry, "insert-text", G_CALLBACK(numeric_field_handler), NULL);
    if ((entry_config.purpose == GTK_INPUT_PURPOSE_PASSWORD) || (entry_config.purpose == GTK_INPUT_PURPOSE_PIN))
        gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

    gtk_entry_set_max_length(GTK_ENTRY(entry), entry_config.max_length);
    // g_print("Entry max length: %d\n", entry_config.max_length);
    if (entry_config.dimensions.height > 0 || entry_config.dimensions.width > 0)
        gtk_widget_set_size_request(entry, entry_config.dimensions.width, entry_config.dimensions.height);

    gtk_widget_set_opacity(entry, entry_config.opacity);
    gtk_entry_set_alignment(GTK_ENTRY(entry), entry_config.alignment);
    gtk_entry_set_has_frame(GTK_ENTRY(entry), entry_config.has_frame);
    gtk_entry_set_overwrite_mode(GTK_ENTRY(entry), entry_config.overwrite_mode);
    if (entry_config.progress_fraction > 0)
        gtk_entry_set_progress_fraction(GTK_ENTRY(entry), entry_config.progress_fraction);

    gtk_entry_set_activates_default(GTK_ENTRY(entry), entry_config.activates_default);

    if (entry_config.progress_pulse_step > 0)
        gtk_entry_set_progress_pulse_step(GTK_ENTRY(entry), entry_config.progress_pulse_step);
    if (entry_config.completion)
        gtk_entry_set_completion(GTK_ENTRY(entry), entry_config.completion);

    // g_print("Entry bg color: %s\n", entry_config.bg_color);
    widget_set_colors(GTK_WIDGET(entry), entry_config.bg_color, entry_config.text_color);
    widget_set_margins(GTK_WIDGET(entry), entry_config.margins);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(entry, entry_config.hexpand);
    gtk_widget_set_vexpand(entry, entry_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(entry, entry_config.halign);
    gtk_widget_set_valign(entry, entry_config.valign);

    // To look at later:
    // gtk_entry_set_cursor_hadjustment(GTK_ENTRY(Myentry), hadjustment);
    //  GdkRGBA bg_color;
    //  gdk_rgba_parse(&bg_color, entry_config.bg_color);
    //  gtk_widget_override_background_color(GTK_WIDGET(entry), GTK_STATE_FLAG_NORMAL, &bg_color);
    //  GdkRGBA text_color;
    //  gdk_rgba_parse(&text_color, entry_config.text_color);
    //  gtk_widget_override_color(GTK_WIDGET(entry), GTK_STATE_FLAG_NORMAL, &text_color);
    return entry;
}

// To look at later: Function to create a completion system for an entry

// GtkEntryCompletion* create_completion_system() {
//     // Create a completion object
//     GtkEntryCompletion *completion = gtk_entry_completion_new();

//     // Create a model with some sample completion data (a simple list of strings)
//     GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
//     gtk_list_store_insert_with_values(store, NULL, -1, 0, "Apple", -1);
//     gtk_list_store_insert_with_values(store, NULL, -1, 0, "Banana", -1);
//     gtk_list_store_insert_with_values(store, NULL, -1, 0, "Cherry", -1);
//     gtk_list_store_insert_with_values(store, NULL, -1, 0, "Grape", -1);
//     gtk_list_store_insert_with_values(store, NULL, -1, 0, "Orange", -1);

//     // Set the model for the completion system
//     gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));

//     // Set the column that will be used for completion (in this case, the first column)
//     gtk_entry_completion_set_text_column(completion, 0);

//     return completion;
// }

EntryConfig *read_entry_config_from_dialog()
{
    EntryConfig *entry_config_ptr = NULL;
    SAFE_ALLOC(entry_config_ptr, EntryConfig, 1);

    EntryConfig entry_config = DEFAULT_ENTRY;

    // Text
    const gchar *text = read_config_value_as_string("text_entry");
    strcpy(entry_config.text, text);

    // Placeholder text
    const gchar *placeholder_text = read_config_value_as_string("placeholder_text_entry");
    strcpy(entry_config.placeholder_text, placeholder_text);

    // Visibility
    gboolean is_visible = read_config_value_as_boolean("visible_switch");
    entry_config.is_visible = is_visible;

    // Has frame
    gboolean has_frame = read_config_value_as_boolean("has_frame_switch");
    entry_config.has_frame = has_frame;

    // Has delete icon
    gboolean has_delete_icon = read_config_value_as_boolean("has_delete_icon_switch");
    entry_config.has_delete_icon = has_delete_icon;

    // Overwrite mode
    gboolean overwrite_mode = read_config_value_as_boolean("overwrite_mode_switch");
    entry_config.overwrite_mode = overwrite_mode;

    // Max length
    gint max_length = read_config_value_as_int("max_length_spin");
    entry_config.max_length = max_length;

    // Alignment
    gfloat alignment = (gfloat)read_config_value_as_double("alignment_spin");
    entry_config.alignment = alignment;

    // Progress fraction
    gfloat progress_fraction = (gfloat)read_config_value_as_double("progress_fraction_spin");
    entry_config.progress_fraction = progress_fraction;

    // Progress pulse step
    gfloat progress_pulse_step = (gfloat)read_config_value_as_double("progress_pulse_step_spin");
    entry_config.progress_pulse_step = progress_pulse_step;

    // Activates default
    gboolean activates_default = read_config_value_as_boolean("activates_default_switch");
    entry_config.activates_default = activates_default;

    // Opacity
    // gfloat opacity = (gfloat)read_config_value_as_double("opacity_spin");
    // entry_config.opacity = opacity;

    // Input purpose
    const gchar *purpose = read_config_value_as_string("type_combo");
    if (strcasecmp(purpose, "number") == 0)
        entry_config.purpose = GTK_INPUT_PURPOSE_NUMBER;
    else if (strcasecmp(purpose, "email") == 0)
        entry_config.purpose = GTK_INPUT_PURPOSE_EMAIL;
    else if (strcasecmp(purpose, "name") == 0)
        entry_config.purpose = GTK_INPUT_PURPOSE_NAME;
    else if (strcasecmp(purpose, "password") == 0)
        entry_config.purpose = GTK_INPUT_PURPOSE_PASSWORD;
    else if (strcasecmp(purpose, "pin") == 0)
        entry_config.purpose = GTK_INPUT_PURPOSE_PIN;
    else
        entry_config.purpose = GTK_INPUT_PURPOSE_FREE_FORM;

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    entry_config.dimensions.width = dimensions->width;
    entry_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    entry_config.margins.top = margins->top;
    entry_config.margins.bottom = margins->bottom;
    entry_config.margins.start = margins->start;
    entry_config.margins.end = margins->end;

    // HAlign
    entry_config.halign = read_align_config("halign_combo");

    // VAlign
    entry_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    entry_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    entry_config.vexpand = vexpand;

    // Background color
    const gchar *bg_color = read_config_value_as_string("bg_color_entry");
    strcpy(entry_config.bg_color, bg_color);

    // Text color
    const gchar *text_color = read_config_value_as_string("color_entry");
    strcpy(entry_config.text_color, text_color);

    memcpy(entry_config_ptr, &entry_config, sizeof(EntryConfig));
    return entry_config_ptr;
}

EntryConfig *read_entry_config_from_widget(GtkWidget *widget)
{
    EntryConfig *entry_config_ptr = NULL;
    SAFE_ALLOC(entry_config_ptr, EntryConfig, 1);

    EntryConfig entry_config = DEFAULT_ENTRY;

    // Text
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(widget));
    strcpy(entry_config.text, text);

    // Placeholder text
    const gchar *placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(widget));
    strcpy(entry_config.placeholder_text, placeholder_text);

    // Visibility
    entry_config.is_visible = gtk_entry_get_visibility(GTK_ENTRY(widget));

    // Has frame
    entry_config.has_frame = gtk_entry_get_has_frame(GTK_ENTRY(widget));

    // Has delete icon
    entry_config.has_delete_icon = gtk_entry_get_icon_name(GTK_ENTRY(widget), GTK_ENTRY_ICON_SECONDARY) != NULL;

    // Overwrite mode
    entry_config.overwrite_mode = gtk_entry_get_overwrite_mode(GTK_ENTRY(widget));

    // Max length
    entry_config.max_length = gtk_entry_get_max_length(GTK_ENTRY(widget));

    // Alignment
    entry_config.alignment = gtk_entry_get_alignment(GTK_ENTRY(widget));

    // Progress fraction
    entry_config.progress_fraction = gtk_entry_get_progress_fraction(GTK_ENTRY(widget));

    // Progress pulse step
    entry_config.progress_pulse_step = gtk_entry_get_progress_pulse_step(GTK_ENTRY(widget));

    // Activates default
    entry_config.activates_default = gtk_entry_get_activates_default(GTK_ENTRY(widget));

    // Input purpose
    entry_config.purpose = gtk_entry_get_input_purpose(GTK_ENTRY(widget));

    // Opacity
    // entry_config.opacity = gtk_widget_get_opacity(widget);

    // Dimensions
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    entry_config.dimensions.width = allocation.width;
    entry_config.dimensions.height = allocation.height;

    // Expand
    entry_config.hexpand = gtk_widget_get_hexpand(widget);
    entry_config.vexpand = gtk_widget_get_vexpand(widget);

    // HAlign
    GtkAlign halign = gtk_widget_get_halign(widget);
    entry_config.halign = halign;

    // VAlign
    GtkAlign valign = gtk_widget_get_valign(widget);
    entry_config.valign = valign;

    // Margins
    Margins margins;
    widget_get_margins(widget, &margins);
    entry_config.margins = margins;

    gchar *property_value = NULL;
    // Background color
    property_value = read_bg_color_from_widget(widget);
    if (property_value)
        strcpy(entry_config.bg_color, property_value);

    // Text color
    property_value = read_text_color_from_widget(widget);
    if (property_value)
        strcpy(entry_config.text_color, property_value);

    memcpy(entry_config_ptr, &entry_config, sizeof(EntryConfig));

    return entry_config_ptr;
}

gchar *write_entry_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "entry", tabs_number);

    // Get the GtkEntry from the view
    GtkEntry *entry = GTK_ENTRY(view->widget);

    // Get the text
    const gchar *text = gtk_entry_get_text(entry);
    if (g_strcmp0(text, "") != 0) // Check if the text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "text=\"%s\"\n", text);
    }

    // Get the placeholder text
    const gchar *placeholder_text = gtk_entry_get_placeholder_text(entry);
    if (g_strcmp0(placeholder_text, "") != 0) // Check if the placeholder text is not the default
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "placeholder_text=\"%s\"\n", placeholder_text);
    }

    // Check if the entry is visible
    gboolean is_visible = gtk_widget_get_visible(GTK_WIDGET(entry));
    if (is_visible != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "is_visible=\"%s\"\n", is_visible ? "true" : "false");
    }

    // Get the input purpose
    GtkInputPurpose purpose = gtk_entry_get_input_purpose(entry);
    if (purpose != GTK_INPUT_PURPOSE_FREE_FORM) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "purpose=\"%s\"\n", purpose == GTK_INPUT_PURPOSE_FREE_FORM ? "free_form" : purpose == GTK_INPUT_PURPOSE_ALPHA  ? "alpha"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_DIGITS   ? "digits"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_NUMBER   ? "number"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_PHONE    ? "phone"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_URL      ? "url"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_EMAIL    ? "email"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_NAME     ? "name"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_PASSWORD ? "password"
                                                                                                    : purpose == GTK_INPUT_PURPOSE_PIN      ? "pin"
                                                                                                                                            : "unknown");
    }

    // Check if the entry has a delete icon
    gboolean has_delete_icon = gtk_entry_get_has_frame(entry);
    if (has_delete_icon != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "has_delete_icon=\"%s\"\n", has_delete_icon ? "true" : "false");
    }

    // Check if the entry has a frame
    gboolean has_frame = gtk_entry_get_has_frame(entry);
    if (has_frame != TRUE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "has_frame=\"%s\"\n", has_frame ? "true" : "false");
    }

    // Get the opacity
    gdouble opacity = gtk_widget_get_opacity(GTK_WIDGET(entry));
    if (opacity != 1.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "opacity=\"%f\"\n", opacity);
    }

    // Check if the entry is in overwrite mode
    gboolean overwrite_mode = gtk_entry_get_overwrite_mode(entry);
    if (overwrite_mode != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "overwrite_mode=\"%s\"\n", overwrite_mode ? "true" : "false");
    }

    // Get the maximum length of the text
    gint max_length = gtk_entry_get_max_length(entry);
    if (max_length != 100) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "max_length=\"%d\"\n", max_length);
    }

    // Get the text alignment
    gfloat alignment = gtk_entry_get_alignment(entry);
    if (alignment != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "alignment=\"%f\"\n", alignment);
    }

    // Get the progress fraction
    gdouble progress_fraction = gtk_entry_get_progress_fraction(entry);
    if (progress_fraction != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "progress_fraction=\"%f\"\n", progress_fraction);
    }

    // Get the progress pulse step
    gdouble progress_pulse_step = gtk_entry_get_progress_pulse_step(entry);
    if (progress_pulse_step != 0.0) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "progress_pulse_step=\"%f\"\n", progress_pulse_step);
    }

    // Check if the entry activates the default widget
    gboolean activates_default = gtk_entry_get_activates_default(entry);
    if (activates_default != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "activates_default=\"%s\"\n", activates_default ? "true" : "false");
    }

    return "entry";
}