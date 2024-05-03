#include <stdlib.h>

typedef struct vector {
  void **data;
  int size;
  int capacity;
} vector;

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
      v->capacity *= 1.25;
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
      v->capacity *= 1.25;
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

void vec_free(vector *v) {
  free(v->data);
  free(v);
}