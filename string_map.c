#include <assert.h>
#include <string.h>

#include "string_map.h"

void string_map_init_with_arena(arena_t* arena, struct string_map_t* map, size_t count) {
    map->data = (struct string_map_entry_t*)arena_alloc(arena, count * sizeof(struct string_map_entry_t));
    map->count = 0;
    map->capacity = count;
}

int string_map_add(struct string_map_t* map, const char* key, void* value) {
    assert(map->count < map->capacity && "No space left in the map");
    map->data[map->count].key = (char*)key;
    map->data[map->count].value = value;
    map->count++;
    return 0;
}

// Replace will act upon the first match of the key.
int string_map_replace(struct string_map_t* map, const char* key, void* value) {
    assert(map->count < map->capacity && "No space left in the map");

    for (size_t i = 0; i < map->count; ++i) {
        if (strncmp(map->data[i].key, key, strlen(key)) == 0) {
            map->data[i].value = value;
            return 0;
        }
    }

    map->data[map->count].key = (char*)key;
    map->data[map->count].value = value;
    map->count++;
    return 0;
}

void* string_map_find_by_key(struct string_map_t* map,
                             struct string_map_entry_t** found_item,
                             const char* key) {
  for (size_t i = 0; i < map->count; ++i) {
    struct string_map_entry_t* item = &map->data[i];
    if (strncmp(item->key, key, strlen(key)) == 0) {
      if (found_item) {
        *found_item = item;
      }
      return item->value;
    }
  }
  return NULL;
}

struct string_map_entry_t *string_map_next_free(struct string_map_t *map) {
  return &map->data[map->count];
}

void string_map_clean(struct string_map_t *map) {
  for (size_t i = 0; i < map->count; ++i) {
    struct string_map_entry_t* entry = &map->data[i];
    entry->key = NULL;
    entry->value = NULL;
  }
  map->count = 0;
}
