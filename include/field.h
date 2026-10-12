#ifndef field_H_
#define field_H_

#include "element.h"
#include <stddef.h>

typedef struct Field Field;
typedef struct Row Row;
typedef _Bool (*Rule)(void *, void *);

struct Row
{
  size_t count;
  size_t cap;
  Element **elements;
};

struct Field {
  char name[128];
  ElementType type;
  Row elements;
};

// Print
_Bool field_print(Field *);
_Bool field_printi(Field *, int i);

// Get and Set
_Bool field_set(Field *, Element *, int index);
Element *field_get(Field *, int index);
Element **field_get_by_rule(Field *, Rule rule);

// Management
_Bool field_append(Field *, Element *);
_Bool field_pop(Field *, int index);
_Bool field_remove(Field *, Element *value);
_Bool field_remove_by_rule(Field *, Rule rule);

// Verification
_Bool field_has(Field *, Element *);
_Bool field_valid(Field *);

// Creation and Destruction
Field *field_create(char *name, ElementType type);
void field_destroy(Field *f);

// Organization
_Bool field_sort(Field *, Rule rule);

#endif /* field_H_ */
