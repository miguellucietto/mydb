#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/element.h"


_Bool element_print(Element *e)
{
  if (!element_valid(e))
    return false;
  
  int status = 0;
  switch (e->type) {

  case TYPE_INTEGER:
    status = printf("%ld: INTEGER\n", e->data.i);
    break;
  case TYPE_SIZE:
    status = printf("%zu: SIZE\n", e->data.s);
    break;
  case TYPE_DECIMAL:
    status = printf("%f: DECIMAL\n", e->data.f);
    break;
  case TYPE_CHARACTER:
    status = printf("'%c': CHARACTER\n", e->data.c);
    break;
  case TYPE_TEXT:
    status = printf("'%s': TEXT\n", e->data.t);
    break;
  case TYPE_BOOL:
    status = printf("%s: BOOLEAN\n", e->data.b ? "true" : "false");
    break;
  default:
    fprintf(stderr, "UNREACHABLE: not valid element\n");
    abort();
  }
  return status > 0;
}

_Bool element_valid(Element *e)
{
  if (!e)
    return false;

  if (e->type > TYPE_BOOL || e->type < TYPE_INTEGER)
    return false;

  return true;
}

_Bool element_equals(Element *e1, Element *e2)
{
  if (!element_valid(e1) || !element_valid(e2)) return false;

  if (!element_cmp_type(e1, e2)) return false;

  switch (e1->type) {
  case TYPE_INTEGER:
    return e1->data.i == e2->data.i;
  case TYPE_SIZE:
    return e1->data.s == e2->data.s;
  case TYPE_DECIMAL:
    return e1->data.f == e2->data.f;
  case TYPE_CHARACTER:
    return e1->data.c == e2->data.c;
  case TYPE_TEXT:
    return strcmp(e1->data.t, e2->data.t) == 0;
  case TYPE_BOOL:
    return e1->data.b == e2->data.b;
  default:
    fprintf(stderr, "UNREACHABLE: invalid type\n");
    abort();
  }
}
_Bool element_cmp_type(Element *e1, Element *e2)
{
  if (!element_valid(e1) || !element_valid(e2)) return false;

  return e1->type == e2->type;
}

Element *element_copy(Element *e)
{
  if (!element_valid(e)) {
    return NULL;
  }

  Element *new = element_create(e->type);
  switch (e->type) {

  case TYPE_INTEGER:
    element_set(new, &e->data.i);
    return new;
  case TYPE_SIZE:
    element_set(new, &e->data.s);
    return new;
  case TYPE_DECIMAL:
    element_set(new, &e->data.f);
    return new;
  case TYPE_CHARACTER:
    element_set(new, &e->data.c);
    return new;
  case TYPE_TEXT:
    element_set(new, e->data.t);
    return new;
  case TYPE_BOOL:
    element_set(new, &e->data.b);
    return new;
  }
}

Element *element_create(const ElementType type)
{
  Element *e = malloc(sizeof(*e));
  if (!e) return NULL;

  e->type = type;
  return e;
}

_Bool element_set(Element *e, void *data)
{
  if (!element_valid(e) || !data) return false;

  switch (e->type) {

  case TYPE_INTEGER:
    e->data.i = *(int *)data;
    return true;
  case TYPE_SIZE:
    e->data.s = *(size_t *)data;
    return true;
  case TYPE_DECIMAL:
    e->data.f = *(double *)data;
    return true;
  case TYPE_CHARACTER:
    e->data.c = *(char *)data;
    return true;
  case TYPE_TEXT:
    e->data.t = (char *)data;
    return true;
  case TYPE_BOOL:
    e->data.b = *(bool *)data;
    return true;
  default:
    fprintf(stderr, "UNREACHABLE: invalid element\n");
    abort();
  }
}

void element_destroy(Element *e)
{
  if (!e)
    return;

  if (e->type == TYPE_TEXT) {
    free(e->data.t);
  }
  free(e);
}
