#include <cstdio>

struct CTCTargetingPlayer {
    char pad[0x3c];
    unsigned char f3c;
    char pad2[3];
    unsigned int f40;
    unsigned int f44;
};

extern "C" unsigned int __fastcall helper_49d870(CTCTargetingPlayer* self);

void __fastcall SetAsPickUpNewLockTarget(CTCTargetingPlayer* self);

static CTCTargetingPlayer* g_seen = 0;
extern "C" unsigned int __fastcall helper_49d870(CTCTargetingPlayer* self)
{
    g_seen = self;
    return 0xDEADBEEF;
}

int main()
{
    CTCTargetingPlayer obj;
    obj.f3c = 0;
    obj.f40 = 123;
    obj.f44 = 0;
    SetAsPickUpNewLockTarget(&obj);
    if (obj.f3c == 1 && obj.f40 == 0 && obj.f44 == 0xDEADBEEF && g_seen == &obj)
        printf("008023a0_TEST PASS\n");
    else
        printf("FAIL f3c=%d f40=%u f44=%x\n", obj.f3c, obj.f40, obj.f44);
    return 0;
}