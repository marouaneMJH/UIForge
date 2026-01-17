#pragma once

#include "./menu.h"
#include "../../properties/property_registry.h"

/*
 * Enum map for GTK_ALIGN
 */
BEGIN_ENUM_MAP(align)
ENUM_MAP_ENTRY("center", GTK_ALIGN_CENTER)
ENUM_MAP_ENTRY("start", GTK_ALIGN_START)
ENUM_MAP_ENTRY("end", GTK_ALIGN_END)
ENUM_MAP_ENTRY("fill", GTK_ALIGN_FILL)
END_ENUM_MAP(align)

/*
 * Nested setters for nested fields
 */
#define NESTED_INT_DEF(field, nested)                                                  \
    static void set_##field(MenuConfig *c, const char *v) { c->nested = conv_int(v); }

#define NESTED_BIND(field) \
    property_registry_register(reg, #field, (GenericSetter)set_##field);

/*
 * Declare nested setters
 */
NESTED_INT_DEF(margin_top, margins.top)
NESTED_INT_DEF(margin_bottom, margins.bottom)
NESTED_INT_DEF(margin_left, margins.start)
NESTED_INT_DEF(margin_right, margins.end)
NESTED_INT_DEF(width, dimensions.width)
NESTED_INT_DEF(height, dimensions.height)

/*
 * Define all setter functions at file scope
 */
DEFINE_SETTERS(MenuConfig,
               STR_DEF(MenuConfig, tooltip)
               STR_DEF(MenuConfig, accel_path)
               INT_DEF(MenuConfig, active_index)
               INT_DEF(MenuConfig, monitor_num)
               BOOL_DEF(MenuConfig, reserve_toggle_size)
               STR_DEF(MenuConfig, bg_color)
               STR_DEF(MenuConfig, text_color)
               ENUM_DEF(MenuConfig, valign, align)
               ENUM_DEF(MenuConfig, halign, align))

/*
 * AUTO_REGISTRY block - only binds, doesn't define
 */
AUTO_REGISTRY(MenuConfig,
              ENTRY(tooltip, STR)
              ENTRY(accel_path, STR)
              ENTRY(active_index, INT)
              ENTRY(monitor_num, INT)
              ENTRY(reserve_toggle_size, BOOL)
              NESTED_BIND(margin_top)
              NESTED_BIND(margin_bottom)
              NESTED_BIND(margin_left)
              NESTED_BIND(margin_right)
              NESTED_BIND(width)
              NESTED_BIND(height)
              ENTRY(bg_color, STR)
              ENTRY(text_color, STR)
              ENTRY(valign, ENUM, align)
              ENTRY(halign, ENUM, align))

/*
 * Export registry
 */
extern PropertyRegistry *MenuConfig_registry;

/*
 * Init wrapper
 */
static inline void register_menu_properties(void)
{
    MenuConfig_init_registry();
}
