// CTCLook::IsTurningBody  @ 0x0082e8f0
// Faithful VC7.1 reconstruction.
//
// Disasm shape:
//   sub esp,8            ; one 8-byte local (only low dword used)
//   lea eax,[esp+0xc]    ; &incoming 8-byte by-value arg
//   push eax
//   lea edx,[esp+4]      ; &local temp
//   push edx
//   add ecx,0x14         ; this -> subobject at +0x14
//   call 0x82f4b0        ; bool sub->Compare(&tmp, &arg)  (caller-cleaned)
//   add esp,8
//   ret 8

struct Vec2
{
    float x;
    float y;
};

// Subobject living at CTCLook+0x14.  Its member takes two pointers and is
// caller-cleaned (cdecl-style arg cleanup) while receiving this in ecx.
struct LookSub
{
    // free helper implemented out-of-line; models the caller-cleaned member
    bool Compare(Vec2* outTmp, Vec2* inArg);
    int pad[8];
};

struct CTCLook
{
    char pad0[0x14];
    LookSub sub;   // at +0x14

    bool IsTurningBody(Vec2 arg);
};

bool CTCLook::IsTurningBody(Vec2 arg)
{
    Vec2 tmp;
    return this->sub.Compare(&tmp, &arg);
}