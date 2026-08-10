// CTCMapwho::OnDie  @ 0x00aecbf0
// push esi; mov esi,ecx; test esi,esi; je end
// call 0xaeca80 (thiscall on esi); push esi; call 0xbfe9bc (cdecl free); add esp,4; pop esi; ret

struct CTCMapwho;

// 0xaeca80 : __fastcall member on esi (ecx = this), no stack args, returns void
void __fastcall CTCMapwho_Cleanup(CTCMapwho* self);
// 0xbfe9bc : cdecl free(void*)
void CTCMapwho_Free(void* p);

struct CTCMapwho {};

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Cleanup(self);
        CTCMapwho_Free(self);
    }
}