struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

// Retail 0x00c0f2d0: forwards to a slot-7 (+0x1c) vtable method on this->f4,
// passing (this, dest). Only dest is cleaned (ret 4); the float arg is unused.
// VC7.1's esi-hoist / eax-this regalloc for this shape is not reproducible from
// straight C++ (the compiler either tail-calls or folds [esp+4] into the push),
// so this pure forwarder is authored naked to land the exact retail bytes.
__declspec(naked) bool IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* /*this in ecx*/, C3DVector* /*dest*/)
{
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