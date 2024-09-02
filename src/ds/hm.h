#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HM_SIZE 5000

struct Map {
  void *value;
  char *key;
  char *purpose;
  struct Map *next;
};

typedef struct {
  size_t current_size;
  size_t size_allocated;
  struct Map **maps;
  int (*custom_hash_cb)(char *);
} HashMap;

HashMap *hm_new(int (*custom_hash_cb)(char *));
void hm_free(void *p);
void hm_insert(HashMap *hm, char *k, void *value, char *purpose);
void hm_allocate_node(char *k, void *value, struct Map *m);
void hm_update_value(struct Map *hm, void *value, char *purpose);
void hm_free_map(struct Map *m);
void hm_resize(HashMap *old_map);
void hm_insert(HashMap *hm, char *k, void *value, char *purpose);
bool hm_has_key(HashMap *hm, char *k);
struct Map *hm_get(HashMap *hm, char *k);