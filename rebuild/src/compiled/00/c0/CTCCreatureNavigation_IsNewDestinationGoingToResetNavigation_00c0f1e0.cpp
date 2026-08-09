struct C3DVector { float x, y, z; };

struct IDelegate {
    // vtable-based
};

struct CTCCreatureNavigation {
    void* vtbl;      // +0
    IDelegate* f4;   // +4
};

// __fastcall bool IsNewDestinationGoingToResetNavigation(this, C3DVector*, float)
// forwards to this->f4->vtbl[6](this, dst)  (returns bool)
__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dst, float f)
{
    (void)self; (void)dst; (void)f;
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