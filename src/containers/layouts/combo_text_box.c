#include "../../../include/containers/layouts/combo_text_box.h"

ViewConfig *configure_combo_text_box_property(
    ComboTextBoxConfig *combo_text_box_config,
    ViewConfig *view_config,
    gchar *property,
    gchar *value)
{
    if (!combo_text_box_config || !property || !value)
        return NULL;

    // Margins
    if (g_strcmp0(property, "margin_top") == 0)
        combo_text_box_config->margins.top = atoi(value);
    else if (g_strcmp0(property, "margin_bottom") == 0)
        combo_text_box_config->margins.bottom = atoi(value);
    else if (g_strcmp0(property, "margin_start") == 0)
        combo_text_box_config->margins.start = atoi(value);
    else if (g_strcmp0(property, "margin_end") == 0)
        combo_text_box_config->margins.end = atoi(value);

    // Dimensions
    else if (g_strcmp0(property, "height") == 0)
        combo_text_box_config->dimensions.height = atoi(value);
    else if (g_strcmp0(property, "width") == 0)
        combo_text_box_config->dimensions.width = atoi(value);

    // Display properties
    else if (g_strcmp0(property, "popup_fixed_width") == 0)
        combo_text_box_config->popup_fixed_width = g_strcmp0(value, "true") == 0;
    else if (g_strcmp0(property, "wrap_width") == 0)
        combo_text_box_config->wrap_width = atoi(value);
    else if (g_strcmp0(property, "popup_shown_rows") == 0)
        combo_text_box_config->popup_shown_rows = atoi(value);
    else if (g_strcmp0(property, "placeholder_text") == 0)
        combo_text_box_config->placeholder_text = g_strdup(value);

    else if (g_strcmp0(property, "default_value") == 0)
        combo_text_box_config->default_value = g_strdup(value);

    else if (g_strcmp0(property, "default_index") == 0)
        combo_text_box_config->default_index = atoi(value);

    else if (g_strcmp0(property, "has_entry") == 0)
        combo_text_box_config->has_entry = (strcmp(value, "true") == 0);

    else if (g_strcmp0(property, "is_editable") == 0)
        combo_text_box_config->is_editable = (strcmp(value, "true") == 0);

    // Style properties
    else if (g_strcmp0(property, "bg_color") == 0)
        strcpy(combo_text_box_config->style.bg_color, value);
    else if (g_strcmp0(property, "color") == 0)
        strcpy(combo_text_box_config->style.color, value);
    else if (g_strcmp0(property, "font_family") == 0)
        strcpy(combo_text_box_config->style.font_family, value);
    else if (g_strcmp0(property, "font_size") == 0)
        combo_text_box_config->style.font_size = atoi(value);
    else if (g_strcmp0(property, "is_bold") == 0)
        combo_text_box_config->style.is_bold = g_strcmp0(value, "true") == 0;
    else if (g_strcmp0(property, "is_italic") == 0)
        combo_text_box_config->style.is_italic = g_strcmp0(value, "true") == 0;

    //  Type
    else if (g_strcmp0(property, "end") == 0)
        combo_text_box_config->type.end = atoi(value);
    else if (g_strcmp0(property, "start") == 0)
        combo_text_box_config->type.start = atoi(value);
    else if (g_strcmp0(property, "type") == 0)
    {
        if (g_strcmp0(value, "month") == 0)
            combo_text_box_config->type.type_counter = MONTH;
        else if (g_strcmp0(value, "day") == 0)
            combo_text_box_config->type.type_counter = DAY;

        else if (g_strcmp0(value, "year") == 0)
            combo_text_box_config->type.type_counter = YEAR;

        else if (g_strcmp0(value, "counter") == 0)
            combo_text_box_config->type.type_counter = COUNTER;
    }

    // Options handling
    else if (strncmp(property, "option", 6) == 0 && property[6] != '\0')
    {
        ComboTextBoxOption *combo_text_box_option = g_new(ComboTextBoxOption, 1);
        combo_text_box_option->key = g_strdup(property + 6);
        combo_text_box_option->value = g_strdup(value);

        if (!combo_text_box_config->options)
            combo_text_box_config->options = g_ptr_array_new_with_free_func(g_free);

        g_ptr_array_add(combo_text_box_config->options, combo_text_box_option);
    }

    if (g_strcmp0(property, "valign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            combo_text_box_config->valign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            combo_text_box_config->valign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            combo_text_box_config->valign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            combo_text_box_config->valign = GTK_ALIGN_FILL;
    }

    if (g_strcmp0(property, "halign") == 0)
    {
        if (g_strcmp0(value, "center") == 0)
            combo_text_box_config->halign = GTK_ALIGN_CENTER;
        else if (g_strcmp0(value, "end") == 0)
            combo_text_box_config->halign = GTK_ALIGN_END;
        else if (g_strcmp0(value, "start") == 0)
            combo_text_box_config->halign = GTK_ALIGN_START;
        else if (g_strcmp0(value, "fill") == 0)
            combo_text_box_config->halign = GTK_ALIGN_FILL;
    }

    // Set view config property
    SET_VIEW_CONFIG_PROPERTY(property, value, view_config);

    return view_config;
}

ViewConfig *init_combo_text_box_config(FILE *index, ComboTextBoxConfig *combo_text_box_config)
{
    return init_generic_config(index, (void *)combo_text_box_config, (ConfigurePropertyCallback)configure_combo_text_box_property);
}

GtkWidget *create_combo_text_box(ComboTextBoxConfig combo_text_box_config)
{
    GtkWidget *combo_text_box;

    // Create the combo box with or without an entry based on configuration
    if (combo_text_box_config.has_entry)
    {
        combo_text_box = gtk_combo_box_text_new_with_entry();
        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo_text_box));

        // Set entry width
        if (combo_text_box_config.dimensions.width > 5 &&
            combo_text_box_config.dimensions.height > 5)
            gtk_widget_set_size_request(entry,
                                        combo_text_box_config.dimensions.width - 5,
                                        combo_text_box_config.dimensions.height - 5);

        // Set editable  permission
        gtk_editable_set_editable(GTK_EDITABLE(entry),
                                  combo_text_box_config.is_editable);

        // Set default text if provided, else set placeholder
        if (combo_text_box_config.default_value && combo_text_box_config.default_value[0] != '\0')
        {
            gtk_entry_set_text(GTK_ENTRY(entry), combo_text_box_config.default_value);
        }
        else if (combo_text_box_config.placeholder_text)
        {
            gtk_entry_set_placeholder_text(GTK_ENTRY(entry), combo_text_box_config.placeholder_text);
        }
    }
    else
    {
        combo_text_box = gtk_combo_box_text_new();
        g_print("label: %d ", combo_text_box_config.default_index);
    }

    /* Verify the type of counter */
    // Add options to the combo box
    if (combo_text_box_config.type.type_counter == NONE &&
        combo_text_box_config.options)
    {
        for (int i = 0; i < combo_text_box_config.options->len; i++)
        {
            ComboTextBoxOption *option = g_ptr_array_index(combo_text_box_config.options, i);
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_text_box), option->key, option->value);
        }
    }
    else if (combo_text_box_config.type.type_counter == COUNTER)
        combo_box_make_interval(combo_text_box,
                                combo_text_box_config.type.start,
                                combo_text_box_config.type.end);

    else if (combo_text_box_config.type.type_counter == YEAR)
        combo_box_make_interval(combo_text_box, 2000, 2025);

    else if (combo_text_box_config.type.type_counter == MONTH)

        combo_box_make_interval(combo_text_box, 1, 12);

    else if (combo_text_box_config.type.type_counter == DAY)
        combo_box_make_interval(combo_text_box, 1, 31);

    // Apply widget dimensions and margins
    gtk_widget_set_size_request(combo_text_box,
                                combo_text_box_config.dimensions.width,
                                combo_text_box_config.dimensions.height);
    widget_set_margins(combo_text_box, combo_text_box_config.margins);

    // Set popup width and wrap width if specified
    gtk_combo_box_set_popup_fixed_width(GTK_COMBO_BOX(combo_text_box),
                                        combo_text_box_config.popup_fixed_width);

    if (combo_text_box_config.wrap_width > 0)
    {
        gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(combo_text_box),
                                     combo_text_box_config.wrap_width);
    }

    // Apply style properties (font, colors, bold, italic)
    if (combo_text_box_config.style.bg_color[0] != '\0' || combo_text_box_config.style.color[0] != '\0')
    {
        widget_set_colors(combo_text_box,
                          combo_text_box_config.style.bg_color,
                          combo_text_box_config.style.color);
    }

    if (combo_text_box_config.style.font_family[0] != '\0')
    {
        widget_set_font_family(combo_text_box,
                               combo_text_box_config.style.font_family);
    }

    if (combo_text_box_config.style.font_size > 0)
    {
        widget_set_font_size(combo_text_box,
                             combo_text_box_config.style.font_size);
    }

    // Apply bold and italic styles if entry exists
    if (combo_text_box_config.has_entry &&
        (combo_text_box_config.style.is_bold || combo_text_box_config.style.is_italic))
    {
        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo_text_box));
        PangoAttrList *attrs = pango_attr_list_new();

        if (combo_text_box_config.style.is_bold)
        {
            pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
        }

        if (combo_text_box_config.style.is_italic)
        {
            pango_attr_list_insert(attrs, pango_attr_style_new(PANGO_STYLE_ITALIC));
        }

        gtk_entry_set_attributes(GTK_ENTRY(entry), attrs);
        pango_attr_list_unref(attrs);
    }

    if (combo_text_box_config.default_index != -1 && !combo_text_box_config.has_entry)
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo_text_box), combo_text_box_config.default_index);

    // Enable or disable cells expand (the parent should be expandable; not important)
    gtk_widget_set_hexpand(combo_text_box, combo_text_box_config.hexpand);
    gtk_widget_set_vexpand(combo_text_box, combo_text_box_config.vexpand);

    // Set alignments
    gtk_widget_set_halign(combo_text_box, combo_text_box_config.halign);
    gtk_widget_set_valign(combo_text_box, combo_text_box_config.valign);
    return combo_text_box;
}

void combo_text_box_remove_all(GtkComboBoxText *combo_text_box)
{
    gtk_combo_box_text_remove_all(combo_text_box);
}

void combo_text_box_remove(GtkComboBoxText *combo_text_box, gint index)
{
    gtk_combo_box_text_remove(combo_text_box, index);
}

void combo_text_box_insert(GtkComboBoxText *combo_text_box,
                           gint index,
                           const gchar *id,
                           const gchar *text)
{
    gtk_combo_box_text_insert(combo_text_box, index, id, text);
}

void combo_box_make_interval(GtkWidget *combo_text_box, int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_text_box), (char *)g_strdup_printf("%0d", i), (char *)g_strdup_printf("%0d", i));
    }
}

void write_combo_text_box_options(GtkComboBox *combo_box, FILE *output_file, View *view, int tabs_number)
{
    GtkTreeModel *model = gtk_combo_box_get_model(combo_box);
    GtkTreeIter iter;
    gchar *option_text;

    g_print("Options\n");
    if (gtk_tree_model_get_iter_first(model, &iter)) // Start from the first row
    {
        int index = 0;
        do
        {
            // Get the text from the first column (assuming it's a simple text column)
            gtk_tree_model_get(model, &iter, 0, &option_text, -1);
            print_tabs(output_file, tabs_number);
            fprintf(output_file, "option%d=\"%s\"\n", index++, option_text);
            g_free(option_text);
        } while (gtk_tree_model_iter_next(model, &iter));
    }
    else
    {
        g_print("The combo box %s has no options.\n", view->view_config->view_id);
    }
}

ComboTextBoxConfig *read_combo_text_box_config_from_dialog()
{
    ComboTextBoxConfig *combo_text_box_config_ptr = NULL;
    SAFE_ALLOC(combo_text_box_config_ptr, ComboTextBoxConfig, 1);

    ComboTextBoxConfig combo_text_box_config = DEFAULT_COMBO_TEXT_BOX_CONFIG;

    // Read has_entry property
    gboolean has_entry = read_config_value_as_boolean("has_entry_switch");
    combo_text_box_config.has_entry = has_entry;

    // Read is_editable property
    gboolean is_editable = read_config_value_as_boolean("editable_switch");
    combo_text_box_config.is_editable = is_editable;

    // Read popup_fixed_width property
    gboolean popup_fixed_width = read_config_value_as_boolean("popup_fixed_width_switch");
    combo_text_box_config.popup_fixed_width = popup_fixed_width;

    // Read wrap_width property
    gint wrap_width = read_config_value_as_int("wrap_width_spin");
    combo_text_box_config.wrap_width = wrap_width;

    // Read popup_shown_rows property
    gint popup_shown_rows = read_config_value_as_int("popup_shown_rows_spin");
    combo_text_box_config.popup_shown_rows = popup_shown_rows;

    // Read placeholder_text property
    const gchar *placeholder_text = read_config_value_as_string("placeholder_entry");
    combo_text_box_config.placeholder_text = g_strdup(placeholder_text);

    // Read default_value property
    const gchar *default_value = read_config_value_as_string("default_value_entry");
    combo_text_box_config.default_value = g_strdup(default_value);

    // Read default_index property
    gint default_index = read_config_value_as_int("default_index_spin");
    combo_text_box_config.default_index = default_index;

    // Read type property
    const gchar *type = read_config_value_as_string("type_combo");
    if (strcasecmp(type, "month") == 0)
        combo_text_box_config.type.type_counter = MONTH;
    else if (strcasecmp(type, "day") == 0)
        combo_text_box_config.type.type_counter = DAY;
    else if (strcasecmp(type, "year") == 0)
        combo_text_box_config.type.type_counter = YEAR;
    else if (strcasecmp(type, "counter") == 0)
        combo_text_box_config.type.type_counter = COUNTER;
    else
        combo_text_box_config.type.type_counter = NONE;

    // Read start property
    gint start = read_config_value_as_int("start_spin");
    combo_text_box_config.type.start = start;

    // Read end property
    gint end = read_config_value_as_int("end_spin");
    combo_text_box_config.type.end = end;

    // Read options property
    const gchar *options = read_config_value_as_string("options_entry");
    int key = 0;
    int index = 0;
    gchar temp[MAX_VALUE_SIZE];

    if (options)
    {
        while (options[key] != '\0' && options[key] != '\n')
        {
            if (options[key] != ';')
            {
                temp[index++] = options[key];
            }
            else
            {
                temp[index] = '\0';

                ComboTextBoxOption *combo_text_box_option = g_new(ComboTextBoxOption, 1);
                combo_text_box_option->key = g_strdup_printf("%d", key);
                combo_text_box_option->value = g_strdup(temp);

                if (!combo_text_box_config.options)
                    combo_text_box_config.options = g_ptr_array_new_with_free_func(g_free);

                g_ptr_array_add(combo_text_box_config.options, combo_text_box_option);

                index = 0;
            }
            key++;
        }

        if (index > 0)
        {
            temp[index] = '\0';

            ComboTextBoxOption *combo_text_box_option = g_new(ComboTextBoxOption, 1);
            combo_text_box_option->key = g_strdup_printf("%d", key);
            combo_text_box_option->value = g_strdup(temp);

            if (!combo_text_box_config.options)
                combo_text_box_config.options = g_ptr_array_new_with_free_func(g_free);

            g_ptr_array_add(combo_text_box_config.options, combo_text_box_option);
        }
    }

    // Dimensions
    Dimensions *dimensions = read_dimensions_config();
    combo_text_box_config.dimensions.width = dimensions->width;
    combo_text_box_config.dimensions.height = dimensions->height;

    // Margins
    Margins *margins = read_margins_config();
    combo_text_box_config.margins.top = margins->top;
    combo_text_box_config.margins.bottom = margins->bottom;
    combo_text_box_config.margins.start = margins->start;
    combo_text_box_config.margins.end = margins->end;

    // HAlign
    combo_text_box_config.halign = read_align_config("halign_combo");

    // VAlign
    combo_text_box_config.valign = read_align_config("valign_combo");

    // HExpand
    gboolean hexpand = read_config_value_as_boolean("hexpand_switch");
    combo_text_box_config.hexpand = hexpand;

    // VExpand
    gboolean vexpand = read_config_value_as_boolean("vexpand_switch");
    combo_text_box_config.vexpand = vexpand;

    memcpy(combo_text_box_config_ptr, &combo_text_box_config, sizeof(ComboTextBoxConfig));
    return combo_text_box_config_ptr;
}

gchar *write_combo_text_box_property(FILE *output_file, View *view, int tabs_number)
{
    if (!output_file || !view)
        return "\0";

    // Write the widget tag and style configuration (without styling elements)
    write_widget_tag_style_view_config(output_file, view, "combo_text_box", tabs_number);

    // Get the GtkComboBoxText from the view
    GtkComboBox *combo_box = GTK_COMBO_BOX(view->widget);

    // Check if the combo box has an entry
    gboolean has_entry = gtk_combo_box_get_has_entry(GTK_COMBO_BOX(combo_box));
    if (has_entry != FALSE) // Check if it's not the default value
    {
        g_print("the combo have a entry");
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "has_entry=\"%s\"\n", has_entry ? "true" : "false");

        // Get the placeholder text
        const gchar *placeholder_text = gtk_entry_get_placeholder_text(
            GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo_box))));

        if (placeholder_text != NULL) // Check if it's not the default value
        {
            print_tabs(output_file, tabs_number + 1);
            fprintf(output_file, "placeholder_text=\"%s\"\n", placeholder_text);
        }

        // Get the default value
        const gchar *default_value = gtk_entry_get_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo_box))));
        if (default_value != NULL && g_strcmp0(default_value, "") != 0) // Check if it's not the default value
        {
            print_tabs(output_file, tabs_number + 1);
            fprintf(output_file, "default_value=\"%s\"\n", default_value);
        }

        // Check if the combo box is editable
        gboolean is_editable = gtk_editable_get_editable(GTK_EDITABLE(gtk_bin_get_child(GTK_BIN(combo_box))));
        if (is_editable != TRUE) // Check if it's not the default value
        {
            print_tabs(output_file, tabs_number + 1);
            fprintf(output_file, "is_editable=\"%s\"\n", is_editable ? "true" : "false");
        }
    }
    // Get the default index
    gint default_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    if (default_index != -1) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "default_index=\"%d\"\n", default_index);
    }

    // Get the wrap width
    gint wrap_width = gtk_combo_box_get_wrap_width(GTK_COMBO_BOX(combo_box));
    if (wrap_width != 1) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "wrap_width=\"%d\"\n", wrap_width);
    }

    // Check if the popup has a fixed width
    gboolean popup_fixed_width = gtk_combo_box_get_popup_fixed_width(GTK_COMBO_BOX(combo_box));
    if (popup_fixed_width != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "popup_fixed_width=\"%s\"\n", popup_fixed_width ? "true" : "false");
    }

    // Get the number of visible rows in the dropdown
    gint popup_shown_rows = gtk_combo_box_get_popup_fixed_width(GTK_COMBO_BOX(combo_box));
    if (popup_shown_rows != 3) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "popup_shown_rows=\"%d\"\n", popup_shown_rows);
    }

    // Get the horizontal expand property
    gboolean hexpand = gtk_widget_get_hexpand(GTK_WIDGET(combo_box));
    if (hexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "hexpand=\"%s\"\n", hexpand ? "true" : "false");
    }

    // Get the vertical expand property
    gboolean vexpand = gtk_widget_get_vexpand(GTK_WIDGET(combo_box));
    if (vexpand != FALSE) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "vexpand=\"%s\"\n", vexpand ? "true" : "false");
    }

    // Get the horizontal alignment
    GtkAlign halign = gtk_widget_get_halign(GTK_WIDGET(combo_box));
    if (halign != GTK_ALIGN_FILL) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "halign=\"%s\"\n", halign == GTK_ALIGN_START ? "start" : halign == GTK_ALIGN_END  ? "end"
                                                                                  : halign == GTK_ALIGN_CENTER ? "center"
                                                                                  : halign == GTK_ALIGN_FILL   ? "fill"
                                                                                                               : "unknown");
    }

    // Get the vertical alignment
    GtkAlign valign = gtk_widget_get_valign(GTK_WIDGET(combo_box));
    if (valign != GTK_ALIGN_FILL) // Check if it's not the default value
    {
        print_tabs(output_file, tabs_number + 1);
        fprintf(output_file, "valign=\"%s\"\n", valign == GTK_ALIGN_START ? "start" : valign == GTK_ALIGN_END  ? "end"
                                                                                  : valign == GTK_ALIGN_CENTER ? "center"
                                                                                  : valign == GTK_ALIGN_FILL   ? "fill"
                                                                                                               : "unknown");
    }

    // write options
    write_combo_text_box_options(combo_box, output_file, view, tabs_number);

    return "combo_text_box";
}