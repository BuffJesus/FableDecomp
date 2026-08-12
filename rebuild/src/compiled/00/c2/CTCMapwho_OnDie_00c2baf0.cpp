// CTCMapwho::OnDie  retail 0x00c2baf0
// Static-style handler: takes CTCMapwho* on the stack, callee-cleanup (ret 4).
//   if (p) { p->Shutdown(); FreeThing(p); }
// call 0xc37da0 = thiscall member on p (ecx=p, no args)
// call 0xbfe9bc = cdecl free fn taking p (push esi; add esp,4)

struct CTCMapwho {
    void Shutdown();   // thiscall member, target 0x00c37da0
};

// cdecl free function, target 0x00bfe9bc
void FreeThing(CTCMapwho* p);

void __stdcall OnDie(CTCMapwho* p)
{
    if (p) {
        p->Shutdown();
        FreeThing(p);
    }
}