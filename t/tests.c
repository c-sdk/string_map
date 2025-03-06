#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "arena.h"
#include "string_map.h"


void test_string_map_init_with_arena() {
  arena_t arena = {0};
  arena_create(&arena, getpagesize());

  struct string_map_t map;
  string_map_init_with_arena(&arena, &map, 10);

  int value1 = 42;
  int value2 = 84;

  string_map_add(&map, "key1", &value1);
  string_map_add(&map, "key2", &value2);

  struct string_map_entry_t* found_item = NULL;

  {
    int* retrieved_entry1 = string_map_find_by_key(&map, &found_item, "key1");
    assert(found_item != NULL && found_item->value == &value1);
    assert(retrieved_entry1 != NULL && *retrieved_entry1 == value1);
  }

  {
    int* retrieved_entry2 = string_map_find_by_key(&map, &found_item, "key2");
    assert(found_item != NULL && found_item->value == &value2);
    assert(retrieved_entry2 != NULL && *retrieved_entry2 == value2);
  }

  arena_free(&arena);
}

void test_string_map_add_and_find_by_key() {
  arena_t arena = {0};
  arena_create(&arena, getpagesize());

  struct string_map_t map;
  string_map_init_with_arena(&arena, &map, 10);

  int value1 = 42;
  int value2 = 84;
  struct string_map_entry_t* found_item = NULL;

  string_map_add(&map, "key1", &value1);
  string_map_add(&map, "key2", &value2);

  {
    int* retrieved_entry1 = string_map_find_by_key(&map, &found_item, "key1");
    assert(found_item != NULL && found_item->value == &value1);
    assert(retrieved_entry1 != NULL && *retrieved_entry1 == value1);
  }

  {
    int* retrieved_entry2 = string_map_find_by_key(&map, &found_item, "key2");
    assert(found_item != NULL && found_item->value == &value2);
    assert(retrieved_entry2 != NULL && *retrieved_entry2 == value2);
  }

  arena_free(&arena);
}

void test_string_map_overwrite() {
  arena_t arena = {0};
  arena_create(&arena, getpagesize());

  struct string_map_t map;
  string_map_init_with_arena(&arena, &map, 10);

  int value1 = 42;
  int value2 = 84;
  struct string_map_entry_t* found_item = NULL;

  string_map_replace(&map, "key1", &value1);
  string_map_replace(&map, "key1", &value2);

  {
    int* retrieved_entry = string_map_find_by_key(&map, &found_item, "key1");
    assert(found_item != NULL && found_item->value == &value2);
    assert(retrieved_entry != NULL && *retrieved_entry == value2);
  }

  arena_free(&arena);
}

void test_string_map_not_found() {
  arena_t arena = {0};
  arena_create(&arena, getpagesize());

  struct string_map_t map;
  string_map_init_with_arena(&arena, &map, 10);

  int value1 = 42;
  struct string_map_entry_t* found_item = NULL;

  string_map_add(&map, "key1", &value1);

  struct string_map_entry_t* retrieved_entry = string_map_find_by_key(&map, &found_item, "key2");

  assert(retrieved_entry == NULL);
  assert(found_item == NULL);

  arena_free(&arena);
}

void test_string_map_clean() {
  arena_t arena = {0};
  arena_create(&arena, getpagesize());

  struct string_map_t map;
  string_map_init_with_arena(&arena, &map, 10);

  char* value = "ok";
  struct string_map_entry_t* found_item = NULL;

  string_map_add(&map, "key1", &value);

  string_map_clean(&map);

  assert(map.count == 0);
  assert(map.capacity == 10);

  for (size_t i = 0; i < map.count; ++i) {
    assert(map.data[i].key == NULL);
    assert(map.data[i].value == NULL);
  }

  arena_free(&arena);
}

int main(void) {
  printf("test string map\n");
  test_string_map_init_with_arena();
  test_string_map_add_and_find_by_key();
  test_string_map_overwrite();
  test_string_map_not_found();
  test_string_map_clean();
  printf("done.\n");
  return 0;
}
