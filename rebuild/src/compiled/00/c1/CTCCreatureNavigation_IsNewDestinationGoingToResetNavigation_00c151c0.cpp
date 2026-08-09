
struct C3DVector { float x, y, z; };
struct Delegate { void* vtbl[8]; };
struct CTCCreatureNavigation { void* field0; Delegate* field4; };

// __fastcall models this-in-ecx; second dummy edx param, then stack params.
__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx_dummy, C3DVector* vec, float f)
{
    (void)self; (void)edx_dummy; (void)vec; (void)f;
    __asm {
        push esi
        mov  esi, dword ptr [esp+8]
        mov  eax, ecx
        mov  ecx, dword ptr [eax+4]
        mov  edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x1c]
        pop  esi
        ret  4
    }
}