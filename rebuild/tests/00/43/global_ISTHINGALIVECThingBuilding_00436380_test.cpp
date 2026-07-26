#include <cstdio>

struct CThing { char pad[0x91]; unsigned char flags; };
struct ThingRef { CThing* p; };

bool __fastcall IS_THING_ALIVE(const ThingRef* self)
{
    CThing* t = self->p;
    if (t != 0 && !(t->flags & 1))
        return true;
    return false;
}

int main()
{
    ThingRef r;
    CThing t;

    r.p = 0;
    if (IS_THING_ALIVE(&r)) { std::printf("FAIL null\n"); return 1; }

    r.p = &t;
    t.flags = 0;
    if (!IS_THING_ALIVE(&r)) { std::printf("FAIL alive\n"); return 1; }

    t.flags = 1;
    if (IS_THING_ALIVE(&r)) { std::printf("FAIL dead\n"); return 1; }

    t.flags = 0xFE;
    if (!IS_THING_ALIVE(&r)) { std::printf("FAIL highbits\n"); return 1; }

    std::printf("_global_00436380_TEST PASS\n");
    return 0;
}