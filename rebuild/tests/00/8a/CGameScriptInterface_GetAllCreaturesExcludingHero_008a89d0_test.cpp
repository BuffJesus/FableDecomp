#include <stdio.h>

struct Ref { long count; int released; int freed; };
struct ScriptThing { unsigned long vftable; long value; Ref* ref; };

static long ModelBridge(ScriptThing* output, long capacity,
                        const ScriptThing* converted, long count,
                        int* reserveCount, int* arrayFreed)
{
    *reserveCount = count;
    long size = 0;
    for (long i = 0; i < count; ++i) {
        ScriptThing temporary = converted[i];
        if (size < capacity) {
            output[size] = temporary;
            if (output[size].ref != 0) ++output[size].ref->count;
        }
        ++size;
        if (temporary.ref != 0 && --temporary.ref->count == 0) {
            temporary.ref->released = 1;
            temporary.ref->freed = 1;
        }
    }
    *arrayFreed = count != 0;
    return size;
}

int main()
{
    Ref retained = {2, 0, 0};
    Ref finalRef = {1, 0, 0};
    ScriptThing input[3] = {{1, 10, &retained}, {1, 20, 0}, {1, 30, &finalRef}};
    ScriptThing output[3] = {};
    int reserved = 0, freed = 0;
    long result = ModelBridge(output, 3, input, 3, &reserved, &freed);
    if (result != 3 || reserved != 3 || freed != 1 || output[0].value != 10 ||
        output[1].value != 20 || output[2].value != 30 || retained.count != 2 ||
        retained.released != 0 || finalRef.count != 1 || finalRef.released != 0)
        return 1;
    reserved = freed = -1;
    result = ModelBridge(output, 3, input, 0, &reserved, &freed);
    if (result != 0 || reserved != 0 || freed != 0)
        return 2;
    puts("GET_ALL_CREATURES_EXCLUDING_HERO_PASS");
    return 0;
}