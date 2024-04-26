typedef struct vector {
  void **data;
  int size;
  int capacity;
} vector;

vector *initializeVector();
void pushLast(vector *v, void *element);
void pushFirst(vector *v, void *element);
void *pop(vector *v);
void *shift(vector *v);
void *at(vector *v, int index);
void freeVector(vector *v);
