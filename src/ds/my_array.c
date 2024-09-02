#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_array.h"

MyArray *new_array() {
  MyArray *arr = malloc(sizeof(MyArray));
  arr->value = NULL;
  assert(arr != NULL);
  arr->size = 0;
  return arr;
}

void push_back(MyArray *arr, void *value, size_t value_size, char *purpose) {
  arr->size++;
  if (arr->value == NULL) {
    arr->value = malloc(sizeof(Value *) * arr->size);
  } else {
    arr->value = realloc(arr->value, sizeof(Value *) * arr->size);
  }
  assert(arr->value != NULL);

  arr->value[arr->size - 1] = malloc(sizeof(Value));
  assert(arr->value[arr->size - 1] != NULL);

  arr->value[arr->size - 1]->value = malloc(value_size);
  assert(arr->value[arr->size - 1]->value != NULL);
  memcpy(arr->value[arr->size - 1]->value, value, value_size);

  arr->value[arr->size - 1]->purpose = purpose == NULL ? NULL : strdup(purpose);
}

Value *get_arr(MyArray *arr, size_t index) {
  if (index >= arr->size) {
    return NULL;
  }
  return arr->value[index];
}

bool has_value(MyArray *arr, size_t index) {
  return get_arr(arr, index) != NULL;
}

void delete_arr(MyArray *arr, size_t index) {
  if (!has_value(arr, index)) {
    return;
  }
  arr->size--;
  free(arr->value[index]);
  arr->value[index] = NULL;
  arr->value[index] = malloc(sizeof(Value));
  assert(arr->value[index] != NULL);
  for (int i = index; i < arr->size; i++) {
    arr->value[i] = arr->value[i + 1];
  }
}

void arr_destroy(MyArray *arr) {
  for (int i = 0; i < arr->size; i++) {
    free(arr->value[i]);
  }
  free(arr->value);
  free(arr);
}