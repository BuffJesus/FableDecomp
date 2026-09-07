#include <stdio.h>
#include <stdlib.h>

struct Value { unsigned long words[4]; };
struct Vector { Value* first; Value* last; Value* end; };

static Vector* Model(Vector* vector, unsigned long count, const Value& seed)
{
    vector->first = vector->last = vector->end = 0;
    if (count == 0) return vector;
    vector->first = (Value*)malloc(count * sizeof(Value));
    vector->end = vector->first + count;
    for (unsigned long i = 0; i < count; ++i) vector->first[i] = seed;
    vector->last = vector->end;
    return vector;
}

int main()
{
    Value seed = {{1, 2, 3, 4}};
    Vector zero, three;
    if (Model(&zero, 0, seed) != &zero || zero.first || zero.last || zero.end)
        return 1;
    if (Model(&three, 3, seed) != &three || three.last - three.first != 3 ||
        three.end != three.last || three.first[0].words[2] != 3 ||
        three.first[2].words[3] != 4)
        return 2;
    free(three.first);
    puts("VECTOR_C2DLINEF_CONSTRUCT_PASS");
    return 0;
}