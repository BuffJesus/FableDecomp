#include <stdio.h>
#include <stdlib.h>

void __stdcall Vector_FreeElements(void** begin, void** end);

int main()
{
    void* values[5];
    values[0] = malloc(8);
    values[1] = 0;
    values[2] = malloc(16);
    values[3] = malloc(1);
    values[4] = 0;
    if (!values[0] || !values[2] || !values[3]) return 1;
    Vector_FreeElements(values, values + 5);
    Vector_FreeElements(values + 5, values + 5);
    printf("VECTOR_FREE_ELEMENTS_TEST PASS\n");
    return 0;
}
