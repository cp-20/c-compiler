#include "vector.h"

#include <stdlib.h>

vector *new_vector() {
  vector *v = (vector *)malloc(sizeof(vector));
  v->capacity = 2;
  v->data = (void **)malloc(sizeof(void *) * v->capacity);
  v->size = 0;
  return v;
}

void vec_push_last(vector *v, void *element) {
  if (v->size == v->capacity) {
    if (v->capacity < 1024) {
      v->capacity *= 2;
    } else {
      v->capacity = v->capacity * 5 / 4;
    }
    v->data = (void **)realloc(v->data, sizeof(void *) * v->capacity);
  }
  v->data[v->size++] = element;
}

void vec_push_first(vector *v, void *element) {
  if (v->size == v->capacity) {
    if (v->capacity < 1024) {
      v->capacity *= 2;
    } else {
      v->capacity = v->capacity * 5 / 4;
    }
    v->data = (void **)realloc(v->data, sizeof(void *) * v->capacity);
  }
  for (int i = v->size; i > 0; i--) {
    v->data[i] = v->data[i - 1];
  }
  v->data[0] = element;
  v->size++;
}

void *vec_pop(vector *v) {
  if (v->size == 0) return NULL;
  return v->data[--v->size];
}

void *vec_shift(vector *v) {
  if (v->size == 0) return NULL;
  void *element = v->data[0];
  for (int i = 0; i < v->size - 1; i++) {
    v->data[i] = v->data[i + 1];
  }
  v->size--;
  return element;
}

void *vec_at(vector *v, int index) {
  if (index < 0 || index >= v->size) return NULL;
  return v->data[index];
}

void *vec_set(vector *v, int index, void *element) {
  if (index < 0 || index >= v->size) return NULL;
  void *old = v->data[index];
  v->data[index] = element;
  return old;
}

void *vec_first(vector *v) {
  if (v->size == 0) return NULL;
  return v->data[0];
}

void *vec_last(vector *v) {
  if (v->size == 0) return NULL;
  return v->data[v->size - 1];
}

void *vec_remove(vector *v, int index) {
  if (index < 0 || index >= v->size) return NULL;
  void *element = v->data[index];
  for (int i = index; i < v->size - 1; i++) {
    v->data[i] = v->data[i + 1];
  }
  v->size--;
  return element;
}

void vec_free(vector *v) {
  free(v->data);
  free(v);
}
