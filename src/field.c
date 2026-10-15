#include "../include/field.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *strdup(const char *s)
{
  if (!s) return NULL;

  size_t len = strlen(s) + 1;
  char *c = malloc(len);

  if (!c) return NULL;

  memcpy(c, s, len);
  return c;
}

size_t indexfor(int i, size_t count)
{
  if (i >= 0 && (size_t) i < count)
      return (size_t)i;

  if (i < 0) {
    i = count + i;
    if (i < 0) {
      fprintf(stderr, "ERROR: index overflow\n");
      abort();
    }
    return (size_t) i;
  }
  
  if ((size_t)i >= count) {
    fprintf(stderr, "ERROR: index overflow\n");
    abort();
  }
  return i;
}


// Print
_Bool field_print(Field *f)
{
  if (!field_valid(f)) return false;
  _Bool status = true;
  printf("%s:\n", f->name);
  for (size_t i = 0; i < f->elements.count; i++) {
    putchar('\t');
    status = field_printi(f, i);
  }
  return status;
}

_Bool field_printi(Field *f, int i)
{
  if (!field_valid(f)) return false;
  return element_print(f->elements.elements[indexfor(i, f->elements.count)]);
}


// Get and Set
_Bool field_set(Field *f, Element *e, int index)
{
  if (!field_valid(f) || !element_valid(e)) return false;
  if (f->type != e->type) return false;

  size_t i = indexfor(index, f->elements.count);

  f->elements.elements[i] = element_copy(e);
  return true;
}

size_t field_get_count(Field *f)
{
  if (!field_valid(f))
    return -1;
  return f->elements.count;
}

Element *field_get(Field *f, int index)
{
  if (!field_valid(f)) return NULL;
  size_t i = indexfor(index, f->elements.count);

  return f->elements.elements[i];
}

size_t field_get_cap(Field *f)
{
  if (!field_valid(f))
    return -1;
  
  return f->elements.cap;
}

Element **field_get_by_rule(Field *f, RuleFn rule)
{
  if (!field_valid(f) || !rule) return NULL;

  size_t cap = f->elements.cap;
  size_t count = 0;
  Element **arr = calloc(cap, sizeof(Element *));
  if (!arr) return NULL;

  for (size_t i = 0; i < f->elements.count; i++) {
    if (rule(f->elements.elements[i])) {
      arr[count++] = f->elements.elements[i];
    }
  }
  arr[count] = NULL;
  return arr;
}


// Management
_Bool field_insert(Field *f, Element *e, int index)
{
  if (!field_valid(f) || !element_valid(e) || f->type != e->type) return false;

  size_t i = indexfor(index, f->elements.count);

  field_reserve(f, f->elements.count + 1);

  memmove(f->elements.elements[i+1], f->elements.elements[i], (f->elements.count - i) * sizeof(Element *));
  f->elements.count++;
  return field_set(f, e, index);
}

_Bool field_append(Field *f, Element *e)
{
  if (!field_valid(f) || !element_valid(e)) return false;

  if (f->elements.count + 1 > f->elements.cap) {
    f->elements.cap *= 2;
    Element **tmp = realloc(f->elements.elements, f->elements.cap * sizeof(*tmp));
    if (!tmp) {
      f->elements.cap /= 2;
      return false;
    }
    f->elements.elements = tmp;
  }
  f->elements.elements[f->elements.count++] = e;
  return true;
}

_Bool field_pop(Field *f, int index)
{
  if (!field_valid(f)) return false;
  if (index == -1 || index == (int) f->elements.count - 1) {
    f->elements.count--;
    return true;
  }
  size_t i = indexfor(index, f->elements.count);

  memmove(f->elements.elements[i], f->elements.elements[i + 1], (f->elements.cap - i - 1) * sizeof(Element *));

  return true;
}

_Bool field_remove(Field *f, Element *value)
{
  if (!field_valid(f) || !element_valid(value)) return false;
  
  for (size_t i = 0; i < f->elements.count; i++) {
    if (element_equals(value, f->elements.elements[i]))
      return field_pop(f, i);
  }
  return false;
}

_Bool field_remove_by_rule(Field *f, RuleFn rule)
{
  if (!field_valid(f) || !rule) return false;

  for (size_t i = 0; i < f->elements.count; i++) {
    if (rule(f->elements.elements[i]))
      return field_pop(f, i);
  }
  return false;
}

_Bool field_clear(Field *f)
{
  if (!field_valid(f)) return false;
  return (f->elements.count = 0, true);
}

_Bool field_shrink_to_fit(Field *f)
{
  if (!field_valid(f))
    return false;
  
  Element **tmp = realloc(f->elements.elements, f->elements.count * sizeof(*tmp));
  if (!tmp) return false;
  
  f->elements.cap = f->elements.count;
  f->elements.elements = tmp;
  return true;
}

_Bool field_reserve(Field *f, size_t amount)
{
  if (!field_valid(f)) {
    return false;
  }
  size_t cap = f->elements.cap;
  if (amount <= cap) return true;
  
  while (amount > cap) {
    cap *= 2;
  }
  Element **tmp = realloc(f->elements.elements, cap * sizeof(*tmp));
  if (!tmp) return false;

  f->elements.cap = cap;
  f->elements.elements = tmp;
  return true;
}

_Bool field_swap(Field *f, int i, int j)
{
  if (!field_valid(f))
    return false;

  size_t index = indexfor(i, f->elements.count);
  size_t jndex = indexfor(j, f->elements.count);
  Element *e = f->elements.elements[index];
  f->elements.elements[index] = f->elements.elements[jndex];
  f->elements.elements[jndex] = e;
  return true;
}

// Verification
_Bool field_has(Field *f, Element *e)
{
  if (!field_valid(f) || !element_valid(e)) {
    return false;
  }

  for (size_t i = 0; i < f->elements.count; i++) {
    if (element_equals(f->elements.elements[i], e)) return true;
  }
  return false;
}

_Bool field_valid(Field *f)
{
  if (!f) return false;
  if (!f->name) return false;
  if (f->elements.cap == 0) return false;
  if (!f->elements.elements) return false;

  for (size_t i = 0; i < f->elements.count; i++) {
    if (f->elements.elements[i]->type != f->type) return false;
  }

  return true;
}

_Bool field_validate(Field *f, _Bool keep)
{
  if (field_valid(f)) return true;

  
  _Bool status = true;
  for (size_t i = 0; i < f->elements.count; i++) {
      if (f->elements.elements[i]->type != f->type) {
          if (keep) {
              f->elements.elements[i]->type = f->type;
          }
          else {
              if (!field_pop(f, i)) {
                status = false;
              }
          }
      }
  }
  return status;
}

// Creation and Destruction
static Row newrow(void)
{
  Row r = {0};
  r.cap = 64;
  r.count = 0;
  r.elements = malloc(sizeof(*r.elements) * r.cap);
  if (!r.elements) {
    fprintf(stderr, "ERROR: Could not allocate row\n");
    abort();
  }
  return r;
}
Field *field_create(char *name, ElementType type)
{
  if (!name) {
    return NULL;
  }

  Field *f = malloc(sizeof(*f));
  if (!f) {
    fprintf(stderr, "Could not allocate field\n");
    return NULL;
  }
  
  f->name = strdup(name);
  f->elements = newrow();
  f->type = type;

  return f;
}

void field_destroy(Field *f)
{
  if (!f)
    return;

  free(f->name);
  for (size_t i = 0; i < f->elements.count; i++) {
    element_destroy(f->elements.elements[i]);
  }
  free(f->elements.elements);
}


// Organization
_Bool field_sort(Field *f, CmpFn rule)
{
  if (!field_valid(f) || !rule) return false;

  for (size_t i = 0; i < f->elements.count; i++) {
      for (size_t j = i; j < f->elements.count - 1; j++) {
          if (rule(f->elements.elements[j], f->elements.elements[j + 1])) {
            field_swap(f, j, j+1);
          }
      }
  }
  return true;
}
