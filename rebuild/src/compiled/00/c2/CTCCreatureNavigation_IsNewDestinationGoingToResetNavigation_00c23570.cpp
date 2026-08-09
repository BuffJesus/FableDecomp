struct C3DVector { float x, y, z; };
struct Inner;
typedef bool (__stdcall *Slot7Fn)(void* self, C3DVector* dest);
struct Inner { Slot7Fn* vtbl; };
struct CTCCreatureNavigation { void* pad0; Inner* field4; };

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* dest)
{
    (void)self; (void)edx; (void)dest;
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