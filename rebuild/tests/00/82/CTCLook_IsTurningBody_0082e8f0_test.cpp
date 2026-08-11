#include <cstdio>

struct Vec2 { float x; float y; };

struct LookSub
{
    bool Compare(Vec2* outTmp, Vec2* inArg);
    int pad[8];
};

struct CTCLook
{
    char pad0[0x14];
    LookSub sub;
    bool IsTurningBody(Vec2 arg);
};

// behaviour model of the callee: returns whether the two components differ
bool LookSub::Compare(Vec2* outTmp, Vec2* inArg)
{
    outTmp->x = inArg->x;
    outTmp->y = inArg->y;
    return (inArg->x != 0.0f) || (inArg->y != 0.0f);
}

bool CTCLook::IsTurningBody(Vec2 arg)
{
    Vec2 tmp;
    return this->sub.Compare(&tmp, &arg);
}

int main()
{
    CTCLook look;
    Vec2 a = { 0.0f, 0.0f };
    Vec2 b = { 1.0f, 0.0f };
    bool r0 = look.IsTurningBody(a);
    bool r1 = look.IsTurningBody(b);
    if (!r0 && r1)
        printf("ISTURNINGBODY_OK\n");
    else
        printf("FAIL r0=%d r1=%d\n", r0, r1);
    return (!r0 && r1) ? 0 : 1;
}