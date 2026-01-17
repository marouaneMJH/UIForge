#pragma once
#include <glib.h>
#include <string.h>
#include <stdlib.h>

/*
 * Conversion helpers
 */
static inline const char *conv_str(const char *v) { return v; }
static inline int conv_int(const char *v) { return atoi(v); }
static inline gboolean conv_bool(const char *v)
{
    return (strcmp(v, "true") == 0 ||
            strcmp(v, "1") == 0 ||
            strcmp(v, "yes") == 0);
}

/*
 * Enum mapping macros
 */
#define BEGIN_ENUM_MAP(name) \
    static struct            \
    {                        \
        const char *key;     \
        int value;           \
    } name##_map[] = {

#define ENUM_MAP_ENTRY(k, v) {k, v},

#define END_ENUM_MAP(name)                                                      \
    }                                                                           \
    ;                                                                           \
    static int name##_lookup(const char *key)                                   \
    {                                                                           \
        for (size_t i = 0; i < sizeof(name##_map) / sizeof(name##_map[0]); ++i) \
            if (strcmp(key, name##_map[i].key) == 0)                            \
                return name##_map[i].value;                                     \
        return 0;                                                               \
    }

/*
 * Generic setter type
 */
typedef void (*GenericSetter)(void *object, const char *value);

typedef struct
{
    GHashTable *table;
} PropertyRegistry;

/*
 * Registry lifecycle
 */
static inline PropertyRegistry *property_registry_new(void)
{
    PropertyRegistry *reg = g_malloc(sizeof(PropertyRegistry));
    reg->table = g_hash_table_new(g_str_hash, g_str_equal);
    return reg;
}

static inline void property_registry_free(PropertyRegistry *reg)
{
    g_hash_table_destroy(reg->table);
    g_free(reg);
}

static inline void property_registry_register(PropertyRegistry *reg,
                                              const char *key,
                                              GenericSetter setter)
{
    g_hash_table_insert(reg->table, (gpointer)key, setter);
}

static inline void property_registry_dispatch(PropertyRegistry *reg,
                                              void *object,
                                              const char *property,
                                              const char *value)
{
    GenericSetter setter = g_hash_table_lookup(reg->table, property);
    if (setter)
        setter(object, value);
}

/*
 * PHASE 1: Setter definitions (file scope)
 */
#define STR_DEF(type, field) \
    static void set_##field(type *c, const char *v) { strcpy(c->field, conv_str(v)); }

#define INT_DEF(type, field) \
    static void set_##field(type *c, const char *v) { c->field = conv_int(v); }

#define BOOL_DEF(type, field) \
    static void set_##field(type *c, const char *v) { c->field = conv_bool(v); }

#define ENUM_DEF(type, field, map) \
    static void set_##field(type *c, const char *v) { c->field = map##_lookup(v); }

/*
 * PHASE 2: Binding to registry (in init function)
 */
#define BIND(field) \
    property_registry_register(reg, #field, (GenericSetter)set_##field);

/*
 * ENTRY(...) frontend - only binds, doesn't define
 */
#define ENTRY(field, kind, ...) ENTRY_IMPL(field, kind, __VA_ARGS__)
#define ENTRY_IMPL(field, kind, ...) BIND(field)

#define STR_ENTRY(field, ...) BIND(field)
#define INT_ENTRY(field, ...) BIND(field)
#define BOOL_ENTRY(field, ...) BIND(field)
#define ENUM_ENTRY(field, map) BIND(field)

/*
 * Macro to define setters at file scope
 */
#define DEFINE_SETTERS(Type, ...) \
    typedef Type TargetType;      \
    __VA_ARGS__

/*
 * AUTO_REGISTRY(Type, ...entries...)
 * Note: Setter functions must be defined with DEFINE_SETTERS before using this
 */
#define AUTO_REGISTRY(Type, ...)                   \
    PropertyRegistry *Type##_registry = NULL;      \
    static inline void Type##_init_registry(void)  \
    {                                              \
        if (Type##_registry)                       \
            return;                                \
        Type##_registry = property_registry_new(); \
        PropertyRegistry *reg = Type##_registry;   \
        typedef Type TargetType;                   \
        __VA_ARGS__                                \
    }
