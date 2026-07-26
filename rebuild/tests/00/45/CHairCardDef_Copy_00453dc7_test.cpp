#include <cstdio>

struct CHairCardDef {
    unsigned char _pad_0x0[0x28];
    int field28;
};

static int g_base_called = 0;
struct CHairCardBase {
    void Copy(CHairCardDef* other) { g_base_called = 1; (void)other; }
};

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CHairCardBase*>(self)->Copy(other);
    self->field28 = other->field28;
}

int main()
{
    CHairCardDef a, b;
    b.field28 = 0x1234;
    a.field28 = 0;
    CHairCardDef_Copy(&a, 0, &b);
    if (a.field28 == 0x1234 && g_base_called == 1) {
        std::printf("CHairCardDef_00453dc7_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL a.field28=%d base=%d\n", a.field28, g_base_called);
    return 1;
}