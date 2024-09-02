#ifndef MY_ARRAY_H
#define MY_ARRAY_H
#include <stdlib.h>
typedef struct {
  void *value;
  char *purpose;
} Value;

typedef struct {
  Value **value;
  size_t size;
} MyArray;

// Function declarations
MyArray *new_array();
void push_back(MyArray *arr, void *value, size_t value_size, char *purpose);
Value *get_arr(MyArray *arr, size_t index);
void delete_arr(MyArray *arr, size_t index);

#endif // MY_ARRAY_H
