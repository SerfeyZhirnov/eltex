#ifndef ELTEX_EX03_DIRECTORY_H_
#define ELTEX_EX03_DIRECTORY_H_

#define MAX_STR_LEN 255
#define COUNT_MEMBERS 20

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct customer {
  char *name;
  char *surname;
  char *patronymic;
  char *phone_number;
} customer;

typedef struct directory {
  size_t customers_count;
  customer *customers;
} directory;

void allocate_memory(directory *);
void add_customer(directory *);
void print_customer(customer *);
void print_customers(directory *);
void delete_customer(directory *);
void find_customer(directory *);
void free_memory(directory *);

#endif  // ELTEX_EX03_DIRECTORY_H_
