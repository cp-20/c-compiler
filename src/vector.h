#pragma once

typedef struct vector {
  void **data;
  int size;
  int capacity;
} vector;

vector *new_vector(void);
void vec_push_last(vector *v, void *element);
void vec_push_first(vector *v, void *element);
void *vec_pop(vector *v);
void *vec_shift(vector *v);
void *vec_at(vector *v, int index);
void *vec_set(vector *v, int index, void *element);
void *vec_first(vector *v);
void *vec_last(vector *v);
void *vec_remove(vector *v, int index);
void vec_free(vector *v);
