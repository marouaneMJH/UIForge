#ifndef XML_WRITER_GLOBALS
#define XML_WRITER_GLOBALS

#include "./../../../global.h"

void print_tabs(FILE *output_file, int tabs_number);

void write_widget_style(FILE *output_file, GtkWidget *widget, int tabs_number);

void write_widget_tag_style_view_config(FILE *output_file, View *view, gchar *tag, int tabs_number);

void write_widget_view_config(FILE *output_file, View *view, int tabs_number);

#endif