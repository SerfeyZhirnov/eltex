#include "func.h"
#include <stdio.h>

int main() {
    printf("%d", sub(sum(5, 4), mult(5, 4)));
    return 0;
}