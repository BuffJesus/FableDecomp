#include <cstdio>

struct CThingSearchTools { int id; };

struct Inner3 { char pad[0x70]; float rate; };
struct Inner2 { char pad[4]; Inner3* p4; };
struct Inner1 { char pad[4]; Inner2* p4; };

struct Sub;
static CThingSearchTools g_result;
static float g_seenScaled = -1.0f;

extern "C" CThingSearchTools* __fastcall SubPeek(Sub* self, float scaled) {
    (void)self;
    g_seenScaled = scaled;
    g_result.id = 42;
    return &g_result;
}

struct CAIStateGroupBase { char pad0[4]; Inner1* f4; char pad8[4]; char sub[4]; };

const float g_const = 8.0f;

CThingSearchTools* __fastcall PeekThingSearchTools(CAIStateGroupBase* self) {
    return SubPeek((Sub*)((char*)self + 0xc), g_const / self->f4->p4->p4->rate);
}

int main() {
    Inner3 i3; i3.rate = 2.0f;
    Inner2 i2; i2.p4 = &i3;
    Inner1 i1; i1.p4 = &i2;
    CAIStateGroupBase s; s.f4 = &i1;

    CThingSearchTools* r = PeekThingSearchTools(&s);
    if (r && r->id == 42 && g_seenScaled == 4.0f) {
        printf("PEEK_OK\n");
    } else {
        printf("PEEK_FAIL scaled=%f\n", g_seenScaled);
    }
    return 0;
}