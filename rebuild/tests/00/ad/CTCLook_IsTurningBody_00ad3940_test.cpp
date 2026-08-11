#include <cstdio>

struct Loc { int x; int y; };
struct Arg { int v; };

struct CTCLook {
    int state;
    bool helper2(Loc* out, Arg* in);
    bool IsTurningBody(Arg inArg);
};

bool CTCLook::IsTurningBody(Arg inArg)
{
    Loc local;
    Loc* pLocal = &local;
    Arg* pArg = &inArg;
    return helper2(pLocal, pArg);
}

// Local stand-in for the forwarded helper (0x514c10). Uses this, writes the
// out-local, and decides "turning" from the argument.
bool CTCLook::helper2(Loc* out, Arg* in)
{
    out->x = state;
    out->y = in->v;
    return (in->v != 0) && (state != 0);
}

int main()
{
    CTCLook look;
    look.state = 5;

    Arg a;
    a.v = 3;
    bool turning = look.IsTurningBody(a);

    Arg b;
    b.v = 0;
    bool notturning = look.IsTurningBody(b);

    printf("turn=%d notturn=%d %s\n", (int)turning, (int)notturning,
           (turning && !notturning) ? "OK_TURN" : "BAD");
    return (turning && !notturning) ? 0 : 1;
}