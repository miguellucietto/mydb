#ifndef table_H_
#define table_H_

#include "field.h"

typedef struct Table Table;
typedef struct Collumn Collumn;

struct Collumn
{
  Field **fields;
  size_t count;
  size_t cap;
};

struct Table
{
  char *name;
  Collumn collumn;
};

// Print
void table_print(Table *);
void table_print_field(Table *, char *);

// Creation and Destruction
Table *table_create(char *name);
void table_destroy(Table *);

// Management
_Bool table_add_field(Table *, Field *);

// Verification
_Bool table_has_field(Table *, char *name);
_Bool table_has_element(Table *, Element *);

_Bool table_valid(Table *); // Check if the table is not NULL and has valid fields
_Bool table_validate(Table *, _Bool keep); // This function may delete some fields and elements

// Get and Set
Field *table_get_field(Table *, char *);


#endif /* table_H_ */
