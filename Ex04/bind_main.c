#include "func.h"
#include <dlfcn.h>
#include <stdio.h>

int main() {
    void *ext_library;
    int (*sum)(int a, int b);
    int (*sub)(int a, int b);
    int (*mult)(int a, int b);

    ext_library = dlopen("./libfunc.so", RTLD_LAZY);
    sum = dlsym(ext_library, "sum");
    sub = dlsym(ext_library, "sub");
    mult = dlsym(ext_library, "mult");

    printf("%d", sub(sum(5, 4), mult(5, 4)));

    return 0;
}
