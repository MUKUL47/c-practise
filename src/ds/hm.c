#include "hm.h"
void hm_free(void *p) {
  if (p != NULL) {
    free(p);
  }
}
void hm_insert(HashMap *hm, char *k, void *value, char *purpose);

struct Map **hm_allocate_size(size_t size) {
  struct Map **m = malloc(sizeof(struct Map *) * size);
  assert(m != NULL);
  memset(m, 0, size * sizeof(struct Map *));
  return m;
}

HashMap *hm_new(int (*custom_hash_cb)(char *)) {
  HashMap *hm = NULL;
  hm = malloc(sizeof(HashMap));
  assert(hm != NULL);
  hm->current_size = HM_SIZE;
  hm->size_allocated = 0;
  hm->maps = hm_allocate_size(hm->current_size);
  if (custom_hash_cb != NULL) {
    hm->custom_hash_cb = custom_hash_cb;
  }
  return hm;
}
int get_hash(HashMap *hm, char *k) {
  int c = 0;
  while (*k != '\0') {
    c = (int)*k++ * 31;
  }
  return c % hm->current_size;
}
int get_key_hash(HashMap *hm, char *k) {
  if (hm->custom_hash_cb != NULL) {
    return hm->custom_hash_cb(k) % hm->current_size;
  }
  return get_hash(hm, k);
}
void hm_allocate_node(char *k, void *value, struct Map *m) {
  m = malloc(sizeof(struct Map));
  assert(m != NULL);
  m->key = strdup(k);
  m->value = value;
  m->next = NULL;
}

struct Map *hm_allocate(struct Map *hm, char *k, void *value, char *purpose) {
  hm = malloc(sizeof(struct Map));
  assert(hm != NULL);
  hm->key = strdup(k);
  hm->value = value;
  hm->next = NULL;
  hm->purpose = purpose != NULL ? strdup(purpose) : NULL;
  return hm;
}

void hm_update_value(struct Map *hm, void *value, char *purpose) {
  hm->value = value;
  hm->purpose = purpose != NULL ? strdup(purpose) : NULL;
}

void hm_free_map(struct Map *m) {
  hm_free(m->key);
  hm_free(m->purpose);
  // hm_free(m->value); //seg fault ??!!
  hm_free(m);
  m = NULL;
}

bool hm_remove(HashMap *hm, char *k) {
  int idx = get_key_hash(hm, k);
  struct Map *m = hm->maps[idx];
  if (m == NULL) {
    return false;
  }
  if (strcmp(m->key, k) == 0) {
    if (m->next == NULL) {
      hm_free_map(m);
    } else {
      struct Map t = *(m->next);
      *m = t;
      m->next = m->next;
      // free ?
    }
    return true;
  }
  struct Map *head = m;
  struct Map *head_next = m->next;
  while (head_next != NULL) {
    struct Map *t_head = head;
    head = head_next;
    if (strcmp(head->key, k) == 0) {
      t_head->next = head->next;
      return true;
    }
    head_next = head_next->next;
  }
  return false;
}

bool hm_has_key(HashMap *hm, char *k) {
  int idx = get_key_hash(hm, k);
  struct Map *m = hm->maps[idx];
  if (m == NULL) {
    return false;
  }
  if (strcmp(m->key, k) == 0) {
    return true;
  }
  struct Map *head = m->next;
  while (head != NULL) {
    if (strcmp(head->key, k) == 0) {
      return true;
    }
    head = head->next;
  }
  return false;
}

struct Map *hm_get(HashMap *hm, char *k) {
  int idx = get_key_hash(hm, k);
  struct Map *m = hm->maps[idx];
  if (m == NULL) {
    return NULL;
  }
  if (strcmp(m->key, k) == 0) {
    return m;
  }
  struct Map *head = m->next;
  while (head != NULL) {
    if (strcmp(head->key, k) == 0) {
      return head;
    }
    head = head->next;
  }
  return NULL;
}

void hm_resize(HashMap *old_map) {
  if (old_map->size_allocated < old_map->current_size) {
    return;
  }
  HashMap *hm_new = NULL;
  hm_new = malloc(sizeof(HashMap));
  assert(hm_new != NULL);
  hm_new->current_size = old_map->current_size * 2;
  hm_new->size_allocated = 0;
  hm_new->maps = hm_allocate_size(hm_new->current_size);
  if (old_map->custom_hash_cb != NULL) {
    hm_new->custom_hash_cb = old_map->custom_hash_cb;
  }
  for (int i = 0; i < old_map->current_size; i++) {
    if (old_map->maps[i] != NULL) {
      struct Map *h = old_map->maps[i];
      while (h != NULL) {
        hm_insert(hm_new, h->key, h->value, h->purpose);
        h = h->next;
      }
    }
  }
  *old_map = *hm_new;
}

void hm_insert(HashMap *hm, char *k, void *value, char *purpose) {
  if (k == NULL) {
    assert(false && "Key is required for insertion");
    return;
  }
  int idx = get_hash(hm, k);
  if (hm->maps[idx] == NULL) {
    hm->size_allocated++;
    hm->maps[idx] = hm_allocate(hm->maps[idx], k, value, purpose);
    // hm_resize(hm);
    return;
  }
  struct Map *head = hm->maps[idx];
  if (strcmp(head->key, k) == 0) {
    hm_update_value(head, value, purpose);
    return;
  }
  struct Map *node = hm->maps[idx]->next;
  while (node != NULL) {
    if (strcmp(node->key, k) == 0) {
      hm_update_value(node, value, purpose);
      return;
    }
    head->next = node;
    head = node;
    node = node->next;
  }
  head->next = hm_allocate(head->next, k, value, purpose);
}