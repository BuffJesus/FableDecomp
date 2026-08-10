struct Inner;
struct CTCCreatureNavigation { Inner* f0; Inner* f4; };
struct C3DVector { float x,y,z; };

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest, float dist)
{
    (void)self; (void)dest; (void)dist;
    __asm {
        push esi
        mov esi, dword ptr [esp+8]
        mov eax, ecx
        mov ecx, dword ptr [eax+4]
        mov edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x1c]
        pop esi
        ret 4
    }
}