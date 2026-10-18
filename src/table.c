#include "../include/table.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Print
void table_print(Table *t)
{
  if (!table_valid(t)) return;

  printf("%s:\n", t->name);
  for (size_t i = 0; i < t->collumn.count; i++) {
    Field *f = t->collumn.fields[i];
    size_t c = field_get_count(f);

    printf("\t%s:\n", f->name);
    
    for (size_t j = 0; j < c; j++) {
      putchar('\t');
      putchar('\t');
      field_printi(f, j);
    }
    putchar('\n');
  }
}

void table_print_field(Table *t, char *name)
{
  if (!table_valid(t) || !name) return;
  if (!table_has_field(t, name)) return;

  Field *f = table_get_field(t, name);

  field_print(f);
}

// Creation and Destruction
Table *table_create(char *name)
{
  if (!name) return NULL;

  Table *t = malloc(sizeof(*t));
  if (!t) return NULL;

  t->name = strdup(name);

  t->collumn.count = 0;
  t->collumn.cap = 16;
  t->collumn.fields = malloc(sizeof(Field *) * 16);
  if (!t->collumn.fields) {
    free(t);
    return NULL;
  }

  return t;
}

void table_destroy(Table *t)
{
  if (!t) return;

  for (size_t i = 0; i < t->collumn.count; i++) {
    field_destroy(t->collumn.fields[i]);
  }
  free(t->name);
  free(t->collumn.fields);
  free(t);
}

// Management
_Bool table_add_field(Table *t, Field *f)
{
  if (!table_valid(t) || !field_valid(f)) return false;

  if (t->collumn.count + 1 > t->collumn.cap) {
    Field **tmp = realloc(t->collumn.fields, t->collumn.cap * 2);
    if (!tmp)
      return false;

    t->collumn.fields = tmp;
    t->collumn.cap *= 2;
  }

  t->collumn.fields[t->collumn.count++] = f;
  return true;
}

// Verification
_Bool table_has_field(Table *t, char *name)
{
  if (!table_valid(t) || !name) return false;

  for (size_t i = 0; i < t->collumn.count; i++) {
    if (strcmp(t->collumn.fields[i]->name, name) == 0) return true;
  }
  return false;
}
_Bool table_has_element(Table *t, Element *e)
{
  if (!table_valid(t) || !element_valid(e)) return false;

  for (size_t i = 0; i < t->collumn.count; i++) {
    if (field_has(t->collumn.fields[i], e)) return true;
  }
  return false;
}

_Bool table_valid(Table *t)
{
  if (!t) return false;
  if (!t->name) return false;
  if (!t->collumn.fields) return false;

  for (size_t i = 0; i < t->collumn.count; i++) {
    if (!field_valid(t->collumn.fields[i])) return false;
  }

  return true;
}

_Bool table_validate(Table *t, _Bool keep)
{
  if (table_valid(t)) return true;

  bool status = true;
  
  for (size_t i = 0; i < t->collumn.count; i++) {
    if (!field_validate(t->collumn.fields[i], keep))
      status = false;
  }

  return status;
}

// Get and Set
Field *table_get_field(Table *t, char *name)
{
  if (!table_valid(t)) return NULL;
  if (!table_has_field(t, name)) return NULL;

  for (size_t i = 0; i < t->collumn.count; i++) {
    if (strcmp(t->collumn.fields[i]->name, name) == 0) {
      return t->collumn.fields[i];
    }
  }
  return NULL;
}


