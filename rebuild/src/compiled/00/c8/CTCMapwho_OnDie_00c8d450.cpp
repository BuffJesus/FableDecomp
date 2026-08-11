// CTCMapwho::OnDie  @ 0x00c8d450  (__fastcall, retail)
// push esi; mov esi,ecx; test esi,esi; je end;
//   call 0xc8d420 (this->PreDie, __fastcall)
//   push esi; call [0x1440154]; add esp,4  (cdecl global fn ptr, arg=this)
// xor eax,eax; pop esi; ret

struct CTCMapwho
{
    void PreDie();      // direct __fastcall member -> call 0xc8d420
    int  OnDie();
};

// global cdecl function pointer -> call dword ptr [0x1440154]
static void (__cdecl *g_pOnMapwhoDie)(CTCMapwho *) = 0;

int CTCMapwho::OnDie()
{
    if (this)
    {
        this->PreDie();
        g_pOnMapwhoDie(this);
    }
    return 0;
}