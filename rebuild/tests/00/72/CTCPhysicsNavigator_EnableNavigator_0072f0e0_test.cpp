#include <cstdio>

struct CTCPhysicsNavigator {
    char pad[0x48];
    unsigned char enabled;
};

void __fastcall CTCPhysicsNavigator_EnableNavigator(CTCPhysicsNavigator* self, void* /*edx*/, unsigned char enable)
{
    self->enabled = enable;
}

int main()
{
    CTCPhysicsNavigator nav;
    nav.enabled = 0;
    CTCPhysicsNavigator_EnableNavigator(&nav, 0, 1);
    if (nav.enabled != 1) { std::printf("FAIL set 1\n"); return 1; }
    CTCPhysicsNavigator_EnableNavigator(&nav, 0, 0);
    if (nav.enabled != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CTCPhysicsNavigator_0072f0e0_TEST PASS\n");
    return 0;
}