// Behaviour: CWorld::GetFrame returns the static CWorld::Frame, whatever it holds.
#include "fable_world.h"
#include <stdio.h>

long CWorld::Frame = 0;

// Test-only accessor for the protected static (a friend-free derived peek).
struct CWorldFramePeek : public CWorld
{
    static void Set(long value) { Frame = value; }
};

int main()
{
    CWorldFramePeek::Set(0);
    if (CWorld::GetFrame() != 0) { printf("FAIL zero\n"); return 1; }
    CWorldFramePeek::Set(0x12345678);
    if (CWorld::GetFrame() != 0x12345678) { printf("FAIL value\n"); return 1; }
    CWorldFramePeek::Set(-7);
    if (CWorld::GetFrame() != -7) { printf("FAIL negative\n"); return 1; }
    printf("OK_0x0049d870\n");
    return 0;
}