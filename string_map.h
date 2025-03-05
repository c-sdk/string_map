#ifndef __AIL_STRING_MAP_H_
#define __AIL_STRING_MAP_H_ 1

#include <stddef.h>
#include "arena.h"

struct string_map_entry_t {
    char* key;
    void* value;
};

struct string_map_t {
    struct string_map_entry_t* data;
    size_t count;
    size_t capacity;
};

void string_map_init_with_arena(arena_t* arena,
                                struct string_map_t* map,
                                size_t count);

int string_map_add(struct string_map_t* map,
                   const char* key,
                   void* value);

int string_map_replace(struct string_map_t* map,
                       const char* key,
                       void* value);

void* string_map_find_by_key(struct string_map_t* map,
                             struct string_map_entry_t** found_item,
                             const char* key);

struct string_map_entry_t* string_map_next_free(struct string_map_t* map);

void string_map_clean(struct string_map_t *map);

#endif // __AIL_STRING_MAP_H_
