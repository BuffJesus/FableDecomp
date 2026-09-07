#include "engine/CHairCardDef.h"
#include <cstdio>


static int g_base_called = 0;
struct CHairCardBase {
    void Copy(CHairCardDef* other) { g_base_called = 1; (void)other; }
};

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CHairCardBase*>(self)->Copy(other);
    self->HairObject = other->HairObject;
}

int main()
{
    CHairCardDef a, b;
    b.HairObject = 0x1234;
    a.HairObject = 0;
    CHairCardDef_Copy(&a, 0, &b);
    if (a.HairObject == 0x1234 && g_base_called == 1) {
        std::printf("CHairCardDef_00453dc7_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL a.HairObject=%d base=%d\n", a.HairObject, g_base_called);
    return 1;
}