#include <stdlib.h>

void *operator new(size_t size)
{
    void *pvVar1 = malloc(size);
    return pvVar1;
}

void *operator new[](size_t size)
{
    void *pvVar1 = malloc(size);
    return pvVar1;
}

void operator delete(void *ptr)
{
    free(ptr);
    return;
}

void operator delete[](void *ptr)
{
    free(ptr);
    return;
}
