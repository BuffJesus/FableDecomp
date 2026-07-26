#include <cstdio>

struct CAnimateThing {
    char pad0[4];
    int curFrame;
    int endFrame;
    char pad1[0x51 - 0xC];
    unsigned char flags;
};

int __fastcall CAnimateThing_IsAnimationFinished(const CAnimateThing* self)
{
    if (self->curFrame != self->endFrame && (self->flags & 2) == 0)
        return 0;
    return 1;
}

int main()
{
    CAnimateThing t;
    for (int i = 0; i < (int)sizeof(t); ++i) ((char*)&t)[i] = 0;

    t.curFrame = 5; t.endFrame = 5; t.flags = 0;
    if (!CAnimateThing_IsAnimationFinished(&t)) { std::printf("FAIL eq\n"); return 1; }

    t.curFrame = 3; t.endFrame = 5; t.flags = 0;
    if (CAnimateThing_IsAnimationFinished(&t)) { std::printf("FAIL neq-noflag\n"); return 1; }

    t.curFrame = 3; t.endFrame = 5; t.flags = 2;
    if (!CAnimateThing_IsAnimationFinished(&t)) { std::printf("FAIL flag2\n"); return 1; }

    t.curFrame = 3; t.endFrame = 5; t.flags = 1;
    if (CAnimateThing_IsAnimationFinished(&t)) { std::printf("FAIL flag1\n"); return 1; }

    t.curFrame = 3; t.endFrame = 5; t.flags = 4;
    if (CAnimateThing_IsAnimationFinished(&t)) { std::printf("FAIL flag4\n"); return 1; }

    std::printf("CAnimateThing_00668a80_TEST PASS\n");
    return 0;
}