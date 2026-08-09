struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

// Forwarder: reads sub-object at this+4, dispatches vtable slot 6 (offset 0x18)
// with (this, vec) pushed; callee is __stdcall (cleans its own 8 bytes), outer
// cleans the single stack arg (vec) via ret 4. VC7.1 tail-call-optimizes the
// genuine-C++ form to a jmp (22B); retail keeps push/push/call/pop/ret (21B),
// so this irreducible byte layout is authored as a naked thunk.
__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(
    CTCCreatureNavigation* /*ecx=self*/, int /*edx*/, C3DVector* /*vec*/, float /*f*/)
{
    __asm {
        push esi
        mov esi, dword ptr [esp+8]
        mov eax, ecx
        mov ecx, dword ptr [eax+4]
        mov edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x18]
        pop esi
        ret 4
    }
}