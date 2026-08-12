// CTCMapwho::OnDie  (retail 0x00c0eb20)
// void __fastcall OnDie(CTCMapwho* this, Obj* p)
// Body: if (p) { p->Cleanup(); FreeObj(p); }
//   this (ecx) is unused; p is first stack arg; ret 4.

struct Obj
{
    void Cleanup();   // __fastcall method -> direct call 0xc1e850
};

// cdecl free function -> push esi; call 0xbfe9bc; add esp,4
void FreeObj(Obj* p);

struct CTCMapwho
{
    void OnDie(Obj* p);
};

void CTCMapwho::OnDie(Obj* p)
{
    if (p)
    {
        p->Cleanup();
        FreeObj(p);
    }
}