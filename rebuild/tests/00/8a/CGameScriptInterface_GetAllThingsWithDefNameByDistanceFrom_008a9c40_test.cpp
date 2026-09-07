#include <stdio.h>

struct Thing { long id; long distance; };
struct ScriptThing { long id; };

static long ModelSearch(Thing** found, long count, ScriptThing* out,
                        long existing, int* reserve, int* freed)
{
    if (count == 0) {
        *freed = found != 0;
        return 0;
    }
    *reserve = count;
    for (long i = 1; i < count; ++i) {
        Thing* value = found[i];
        long j = i;
        while (j != 0 && found[j - 1]->distance > value->distance) {
            found[j] = found[j - 1];
            --j;
        }
        found[j] = value;
    }
    for (long i = 0; i < count; ++i)
        out[existing + i].id = found[i]->id;
    *freed = 1;
    return existing + count;
}

int main()
{
    Thing a = {1, 30}, b = {2, 10}, c = {3, 20};
    Thing* found[3] = {&a, &b, &c};
    ScriptThing out[4] = {{99}, {0}, {0}, {0}};
    int reserve = 0, freed = 0;
    long result = ModelSearch(found, 3, out, 1, &reserve, &freed);
    if (result != 4 || reserve != 3 || freed != 1 || out[0].id != 99 ||
        out[1].id != 2 || out[2].id != 3 || out[3].id != 1)
        return 1;
    reserve = freed = -1;
    if (ModelSearch(0, 0, out, 0, &reserve, &freed) != 0 || freed != 0)
        return 2;
    puts("GET_THINGS_BY_DEF_DISTANCE_PASS");
    return 0;
}