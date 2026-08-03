#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char useAlternate;   // +0xc
    char pad1[0x7c - 0xd];
    float sensA;         // +0x7c
    char pad2[0x130 - 0x80];
    float sensB;         // +0x130
};

float __fastcall GetCameraSensitivity(CUserProfileManager* self)
{
    if (self->useAlternate)
        return self->sensB;
    return self->sensA;
}

// call helper: this=ecx, float return in st(0)
static float call_it(CUserProfileManager* obj)
{
    float result;
    void* fn = (void*)&GetCameraSensitivity;
    __asm {
        mov ecx, obj
        call fn
        fstp result
    }
    return result;
}

int main()
{
    CUserProfileManager o;
    o.sensA = 1.5f;
    o.sensB = 9.25f;

    o.useAlternate = 0;
    float r0 = call_it(&o);

    o.useAlternate = 1;
    float r1 = call_it(&o);

    bool ok = (r0 == 1.5f) && (r1 == 9.25f);

    // also confirm offsets are what we expect
    if ((char*)&o.useAlternate - (char*)&o != 0xc) ok = false;
    if ((char*)&o.sensA - (char*)&o != 0x7c) ok = false;
    if ((char*)&o.sensB - (char*)&o != 0x130) ok = false;

    if (ok) {
        printf("OK_0x0054a510 r0=%f r1=%f\n", r0, r1);
        return 0;
    }
    printf("FAIL r0=%f r1=%f\n", r0, r1);
    return 1;
}