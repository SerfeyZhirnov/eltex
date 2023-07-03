#include "directory.h"

void allocate_memory(directory *data) {
  data->customers_count = 0;
  data->customers = (customer *)calloc(COUNT_MEMBERS, sizeof(customer));
  for (size_t i = 0; i < COUNT_MEMBERS; ++i) {
    data->customers[i].name = (char *)calloc(MAX_STR_LEN, sizeof(char));
    data->customers[i].surname = (char *)calloc(MAX_STR_LEN, sizeof(char));
    data->customers[i].patronymic = (char *)calloc(MAX_STR_LEN, sizeof(char));
    data->customers[i].phone_number = (char *)calloc(MAX_STR_LEN, sizeof(char));
  }
}

void add_customer(directory *data) {
  if (data == NULL) {
    printf("Memory was not allocated\n");
    return;
  }
  if (data->customers_count >= COUNT_MEMBERS) {
    printf("Overflow directory!\n");
    return;
  }

  fflush(stdin);

  const size_t cur_id = data->customers_count;
  printf("Enter surname: ");
  fgets(data->customers[cur_id].surname,
        sizeof(data->customers[cur_id].surname), stdin);
  data->customers[cur_id]
      .surname[strcspn(data->customers[cur_id].surname, "\n")] = '\0';

  printf("Enter name: ");
  fgets(data->customers[cur_id].name, sizeof(data->customers[cur_id].name),
        stdin);
  data->customers[cur_id].name[strcspn(data->customers[cur_id].name, "\n")] =
      '\0';

  printf("Enter patronymic: ");
  fgets(data->customers[cur_id].patronymic,
        sizeof(data->customers[cur_id].patronymic), stdin);
  data->customers[cur_id]
      .patronymic[strcspn(data->customers[cur_id].patronymic, "\n")] = '\0';

  printf("Enter phone number: ");
  fgets(data->customers[cur_id].phone_number,
        sizeof(data->customers[cur_id].phone_number), stdin);
  data->customers[cur_id]
      .phone_number[strcspn(data->customers[cur_id].phone_number, "\n")] = '\0';

  ++data->customers_count;

  printf("Customer added successfully\n");
}

void print_customer(customer *data) {
  printf("%s\t%s\t%s\t%s\n", data->surname, data->name, data->patronymic,
         data->phone_number);
}

void print_customers(directory *data) {
  printf("Surname\tName\tPatronymic\tPhone Number\n");
  for (size_t i = 0; i < data->customers_count; ++i) {
    print_customer(&data->customers[i]);
  }
}

void delete_customer(directory *data) {
  printf("Enter index of customer to delete: ");
  size_t index;
  scanf("%zu", &index);

  if (index >= data->customers_count) {
    printf("Castomer with this id not found\n");
    return;
  }

  if (index != data->customers_count - 1) {
    for (; index < data->customers_count; ++index) {
      memcpy(data->customers[index].name, data->customers[index + 1].name,
             MAX_STR_LEN);
      memcpy(data->customers[index].patronymic,
             data->customers[index + 1].patronymic, MAX_STR_LEN);
      memcpy(data->customers[index].phone_number,
             data->customers[index + 1].phone_number, MAX_STR_LEN);
      memcpy(data->customers[index].surname, data->customers[index + 1].surname,
             MAX_STR_LEN);
    }
  }
  --data->customers_count;
}

void find_customer(directory *data) {
  printf("Enter 0 to find by name\n");
  printf("Enter 1 to find by surname\n");
  printf("Enter 2 to find by patronymic\n");
  printf("Enter 3 to find by phone_number\n");

  size_t param;
  scanf("%zu", &param);

  if (param > 3) {
    return;
  }

  char *value = (char *)calloc(MAX_STR_LEN, sizeof(char));
  printf("Enter a search value: ");
  scanf("%s", value);

  printf("Surname\tName\tPatronymic\tPhone Number\n");
  for (size_t i = 0; i < data->customers_count; ++i) {
    if ((param == 0 && strcmp(data->customers[i].name, value) == 0) ||
        (param == 1 && strcmp(data->customers[i].surname, value) == 0) ||
        (param == 2 && strcmp(data->customers[i].patronymic, value) == 0) ||
        (param == 3 && strcmp(data->customers[i].phone_number, value) == 0)) {
      print_customer(&data->customers[i]);
    }
  }
  free(value);
}

void free_memory(directory *data) {
  data->customers_count = 0;
  for (size_t i = 0; i < COUNT_MEMBERS; ++i) {
    free(data->customers[i].name);
    free(data->customers[i].surname);
    free(data->customers[i].patronymic);
    free(data->customers[i].phone_number);
  }
  free(data->customers);
  data->customers = NULL;
}
