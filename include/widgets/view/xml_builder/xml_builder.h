#ifndef XML_BUILDER
#define XML_BUILDER

#include "./../../../index.h"
#include "xml_writer_globals.h"

void write_widget(FILE *output_file, View *view, int tabs_number);

gboolean is_container(View *widget);

gchar *write_widget_property(FILE *output_file, View *view, int tabs_number);

void write_widget_close_tag(FILE *output_file, View *view, gchar *tag, int tabs_number);

void build_xml(gchar *file_name, gboolean dialog_is_showed);

#endif
