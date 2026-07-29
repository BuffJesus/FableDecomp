#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0(
    void* physics, void*, const void* right_handed_set);

struct Physics
{
    unsigned char prefix[0x28];
    unsigned long facing[3];
    unsigned long suffix;
};

struct RightHandedSet
{
    unsigned long angles[3];
};

static int g_set_moving_calls;
static void* g_set_moving_physics;
static long g_set_moving_value;

extern "C" void __fastcall
CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0_SetAsMoving(
    void* physics, void*, long moving)
{
    ++g_set_moving_calls;
    g_set_moving_physics = physics;
    g_set_moving_value = moving;
}

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    Physics physics;
    RightHandedSet source;

    memset(&physics, 0x5a, sizeof(physics));
    source.angles[0] = 0x3f800000;
    source.angles[1] = 0xbf000000;
    source.angles[2] = 0x7fc00001;
    g_set_moving_calls = 0;
    g_set_moving_physics = 0;
    g_set_moving_value = 0;

    CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0(
        &physics, 0, &source);
    ok &= Check(physics.facing[0] == source.angles[0] &&
                physics.facing[1] == source.angles[1] &&
                physics.facing[2] == source.angles[2],
                "all three facing words must be copied exactly");
    ok &= Check(physics.prefix[0] == 0x5a &&
                physics.suffix == 0x5a5a5a5a,
                "adjacent object state must remain unchanged");
    ok &= Check(g_set_moving_calls == 1 &&
                g_set_moving_physics == &physics &&
                g_set_moving_value == 1,
                "method must tail-call SetAsMoving(true) on this");

    source.angles[0] = 0;
    source.angles[1] = 0xffffffff;
    source.angles[2] = 0x80000000;
    CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0(
        &physics, 0, &source);
    ok &= Check(physics.facing[0] == 0 &&
                physics.facing[1] == 0xffffffff &&
                physics.facing[2] == 0x80000000,
                "copy must preserve arbitrary bit patterns");
    ok &= Check(g_set_moving_calls == 2,
                "each update must mark the body moving");

    if (!ok)
        return 1;

    puts("PASS CTCPhysicsRigidBody::SetFacingAnglesFromRHSet 0x0066F9B0");
    return 0;
}
