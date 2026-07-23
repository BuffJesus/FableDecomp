struct CThing;
struct CPlayer;
extern "C" void* g_manager;
extern "C" void* CThing_Lookup_target();
__declspec(naked) CPlayer* __fastcall CThing_GetPlayer(CThing* self)
{
    (void)self;
    __asm {
        mov eax, ecx
        movsx edx, byte ptr [eax+0x90]
        mov ecx, dword ptr [g_manager]
        mov ecx, dword ptr [ecx+0x30]
        push edx
        call CThing_Lookup_target
        ret
    }
}