// CTCLook::IsTurningBody @ 0x0075dcf0
// sub esp,8 ; lea eax,[esp+0xc]=&arg ; push eax ; lea edx,[esp+4]=&local ; push edx
// add ecx,0xc0 ; call 0x512da0 (thiscall self=this+0xc0, caller-cleaned 2 stack args)
// add esp,8 ; ret 4  -> 4-byte by-value arg, forwards helper(&local,&arg).

struct Loc { int x; int y; };
struct Arg { int v; };

struct SubObj {
    bool Compute(Loc* out, Arg* in);
};

struct CTCLook {
    char pad[0xc0];
    SubObj sub;   // at +0xc0
    bool IsTurningBody(Arg arg);
};

bool CTCLook::IsTurningBody(Arg arg)
{
    Loc local;
    return this->sub.Compute(&local, &arg);
}