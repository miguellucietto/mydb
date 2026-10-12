#ifndef element_H_
#define element_H_

#include <stdio.h>
#include <stdint.h>

typedef struct Element Element;

typedef enum {
  TYPE_INTEGER,
  TYPE_SIZE,
  TYPE_DECIMAL,
  TYPE_CHARACTER,
  TYPE_TEXT,
  TYPE_BOOL,
} ElementType;

struct Element {
  ElementType type;

  union {
    int64_t i;
    size_t s;
    double f;
    char c;
    char *t;
    _Bool b;
  } data;
};



_Bool element_print(Element *);

_Bool element_valid(Element *);

_Bool element_equals(Element *, Element *);
_Bool element_cmp_type(Element *, Element *);

Element *element_create(ElementType);
void element_destroy(Element *);

_Bool element_set(Element *, void *data);

#endif /* element_H_ */
