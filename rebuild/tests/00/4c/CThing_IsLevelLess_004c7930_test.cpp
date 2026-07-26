#include <cstdio>

struct CLevelObj { char pad[0x41]; unsigned char flags; };
struct CThing { char pad[0x70]; CLevelObj* level; };

bool __fastcall CThing_IsLevelLess(const CThing* self)
{
    const CLevelObj* lvl = self->level;
    bool r = false;
    if (lvl != 0 && (lvl->flags & 0x8) != 0)
        r = true;
    return r;
}

int main()
{
    CThing t;
    CLevelObj l;
    for (int i = 0; i < (int)sizeof(t); ++i) ((char*)&t)[i] = 0;
    for (int i = 0; i < (int)sizeof(l); ++i) ((char*)&l)[i] = 0;

    t.level = 0;
    if (CThing_IsLevelLess(&t) != false) { std::printf("FAIL null\n"); return 1; }

    t.level = &l;
    l.flags = 0x00;
    if (CThing_IsLevelLess(&t) != false) { std::printf("FAIL noflag\n"); return 1; }

    l.flags = 0x08;
    if (CThing_IsLevelLess(&t) != true) { std::printf("FAIL flag8\n"); return 1; }

    l.flags = 0xF7;
    if (CThing_IsLevelLess(&t) != false) { std::printf("FAIL mask\n"); return 1; }

    l.flags = 0xFF;
    if (CThing_IsLevelLess(&t) != true) { std::printf("FAIL all\n"); return 1; }

    std::printf("CThing_004c7930_TEST PASS\n");
    return 0;
}