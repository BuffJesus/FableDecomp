// CTCLook__IsTurningBody @ 00ad3940
// bool CTCLook::IsTurningBody(Arg inArg)
//
// Retail forwards to a thiscall member helper (0x514c10), passing the address
// of an 8-byte stack local and the address of the incoming by-value argument.
// this flows through ecx unchanged (no ecx setup), edx holds the local address
// temp, and the 8-byte local reserve is reclaimed by add esp,8 while the helper
// cleans its own two stack args (thiscall). Returns the helper bool.

struct Loc { int x; int y; };
struct Arg { int v; };

struct CTCLook {
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