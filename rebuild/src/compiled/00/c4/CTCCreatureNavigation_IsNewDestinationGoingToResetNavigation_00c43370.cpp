struct C3DVector { float x, y, z; };

// Forwards to  this->nav(+0x50)->vtbl[0x30](C3DVector*, float).
// Retail re-pushes both incoming args rather than tail-calling, so the exact
// byte layout is modelled with a naked thunk (VC7.1 TCO-folds the natural C++
// form to a jmp).
__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation_v2(void* /*ecx self*/, int /*edx*/, C3DVector* /*v*/, float /*f*/) {
    __asm {
        mov edx, dword ptr [esp+8]
        mov ecx, dword ptr [ecx+0x50]
        mov eax, dword ptr [ecx]
        push edx
        mov edx, dword ptr [esp+8]
        push edx
        call dword ptr [eax+0x30]
        ret 8
    }
}