#include <stdio.h>

// Behaviour model: CThingNoiseDef::Copy forwards to base Copy with src,
// then copies the +0x70 submember from src+0x70.

struct Base { int tag; int payload; };
struct Sub  { int tag; int payload; };

struct Thing {
    Base base;      // occupies the base region
    Sub  sub;       // the +0x70 submember region
};
struct Src {
    Base base;
    Sub  sub;
};

int g_baseCalled = 0;
int g_subCalled = 0;

void baseCopy(Base* self, const Base* src) { self->tag = src->tag; self->payload = src->payload; g_baseCalled++; }
void subCopy(Sub* self, const Sub* src)   { self->tag = src->tag; self->payload = src->payload; g_subCalled++; }

void thingCopy(Thing* self, const Src* src) {
    baseCopy(&self->base, &src->base);
    subCopy(&self->sub, &src->sub);
}

int main() {
    Src s; s.base.tag = 11; s.base.payload = 22; s.sub.tag = 33; s.sub.payload = 44;
    Thing t; t.base.tag = 0; t.base.payload = 0; t.sub.tag = 0; t.sub.payload = 0;
    thingCopy(&t, &s);
    if (t.base.tag == 11 && t.base.payload == 22 &&
        t.sub.tag == 33 && t.sub.payload == 44 &&
        g_baseCalled == 1 && g_subCalled == 1) {
        printf("COPY_OK\n");
    } else {
        printf("COPY_FAIL\n");
    }
    return 0;
}