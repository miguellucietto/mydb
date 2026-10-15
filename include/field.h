#ifndef field_H_
#define field_H_

#include "element.h"
#include <stddef.h>

typedef struct Field Field;
typedef struct Row Row;
typedef _Bool (*RuleFn)(void *);
typedef _Bool (*CmpFn)(void *, void *);

struct Row
{
  size_t count;
  size_t cap;
  Element **elements;
};

struct Field {
  char *name;
  ElementType type;
  Row elements;
};


// Utils
char *strdup(const char *);
size_t indexfor(int i, size_t count);

// Print
_Bool field_print(Field *);
_Bool field_printi(Field *, int i);

// Get and Set
_Bool field_set(Field *, Element *, int index);
Element *field_get(Field *, int index);
Element **field_get_by_rule(Field *, RuleFn rule); // last element is NULL to indicate the end of the array
size_t field_get_count(Field *);
size_t field_get_cap(Field *);

// Management
_Bool field_append(Field *, Element *);
_Bool field_pop(Field *, int index);
_Bool field_remove(Field *, Element *value);
_Bool field_remove_by_rule(Field *, RuleFn rule);
_Bool field_clear(Field *);

_Bool field_shrink_to_fit(Field *);
_Bool field_reserve(Field *, size_t);

_Bool field_swap(Field *, int i, int j);

// Verification
static inline _Bool field_empty(Field *f) { return (f && f->elements.count == 0); }

_Bool field_has(Field *, Element *);
_Bool field_valid(Field *);
_Bool field_validate(Field *, _Bool keep);

// Creation and Destruction
Field *field_create(char *name, ElementType type);
void field_destroy(Field *f);

// Organization
_Bool field_sort(Field *, CmpFn rule);

#endif /* field_H_ */
