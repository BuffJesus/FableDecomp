#include <cstdio>

struct CThingInner { char pad[0x91]; unsigned char flags; };
struct CThingHolder { CThingInner* inner; };

bool __fastcall IS_THING_ALIVE(const CThingHolder* self)
{
    const CThingInner* p = self->inner;
    if (p != 0 && (p->flags & 1) == 0)
        return true;
    return false;
}

int main()
{
    CThingInner alive; alive.flags = 0;
    CThingInner dead;  dead.flags = 1;
    CThingHolder h;

    h.inner = 0;
    if (IS_THING_ALIVE(&h)) { std::printf("FAIL null\n"); return 1; }

    h.inner = &alive;
    if (!IS_THING_ALIVE(&h)) { std::printf("FAIL alive\n"); return 1; }

    h.inner = &dead;
    if (IS_THING_ALIVE(&h)) { std::printf("FAIL dead\n"); return 1; }

    alive.flags = 2;
    h.inner = &alive;
    if (!IS_THING_ALIVE(&h)) { std::printf("FAIL bit0only\n"); return 1; }

    std::printf("_global_0040dce0_TEST PASS\n");
    return 0;
}