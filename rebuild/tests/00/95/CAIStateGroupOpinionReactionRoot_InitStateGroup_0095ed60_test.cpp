#include <stdio.h>
#include <string.h>

struct Obj {
    unsigned char pad[0x18];
    int field_18;
    unsigned char states[79];
    unsigned char guard;
};

void __fastcall InitStateGroup(Obj* self, int edx_dummy, const void* world, void* creature)
{
    unsigned char* d = self->states;
    self->field_18 = 0;
    for (int i = 0; i < 79; ++i) d[i] = 1;
}

int main()
{
    Obj o;
    memset(&o, 0xEE, sizeof(o));
    o.field_18 = 0x12345678;
    o.guard = 0xEE;
    int dummyWorld = 0, dummyCreature = 0;
    InitStateGroup(&o, 0, &dummyWorld, &dummyCreature);

    if (o.field_18 != 0) { printf("BAD field_18\n"); return 1; }
    for (int i = 0; i < 79; i++) {
        if (o.states[i] != 1) { printf("BAD state %d\n", i); return 1; }
    }
    if (o.guard != 0xEE) { printf("BAD guard overwrite\n"); return 1; }
    printf("PARITY_OK\n");
    return 0;
}