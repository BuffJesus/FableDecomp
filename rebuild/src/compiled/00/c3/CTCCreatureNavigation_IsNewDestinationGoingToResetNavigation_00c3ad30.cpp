struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest, float tolerance)
{
    __asm {
        push esi
        mov  esi, dword ptr [esp+8]
        mov  eax, ecx
        mov  ecx, dword ptr [eax+4]
        mov  edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x18]
        pop  esi
        ret  4
    }
}