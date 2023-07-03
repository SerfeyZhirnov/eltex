#include "directory.h"

int main() {
  directory data;
  allocate_memory(&data);

  size_t mode;
  do {
    printf("Enter 0 to exit\n");
    printf("Enter 1 to add customer\n");
    printf("Enter 2 to print customers\n");
    printf("Enter 3 to delete customer\n");
    printf("Enter 4 to find customer\n");

    scanf("%zu", &mode);

    switch (mode) {
      case 1:
        add_customer(&data);
        break;
      case 2:
        print_customers(&data);
        break;
      case 3:
        delete_customer(&data);
        break;
      case 4:
        find_customer(&data);
        break;
      default:
        break;
    }
  } while (mode != 0);

  free_memory(&data);
  return 0;
}