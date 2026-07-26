#include <cstdio>

struct CTCPhysicsNavigator { char pad[0x3c]; char enabled; };

void __fastcall CTCPhysicsNavigator_EnableNavigator(CTCPhysicsNavigator* self, int /*edx*/, char enable)
{
    self->enabled = enable;
}

int main()
{
    CTCPhysicsNavigator n;
    n.enabled = 0;
    CTCPhysicsNavigator_EnableNavigator(&n, 0, 1);
    if (n.enabled != 1) { std::printf("FAIL set 1\n"); return 1; }
    CTCPhysicsNavigator_EnableNavigator(&n, 0, 0);
    if (n.enabled != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CTCPhysicsNavigator_006bd830_TEST PASS\n");
    return 0;
}