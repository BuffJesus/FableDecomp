// Front-end history deque iterator equality helper @ 0x0059A9C0.
//
// The retail iterator ABI stores current at +0, first at +4, last at +8, and
// node at +0x0C.  Equality is intentionally defined only by the current slot.

struct FrontEndHistoryDequeIterator_0059a9c0
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C" __declspec(naked)
bool __cdecl FrontEndHistoryDequeIterator_Equal_0059a9c0(
    const FrontEndHistoryDequeIterator_0059a9c0*,
    const FrontEndHistoryDequeIterator_0059a9c0*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [esp + 8]
        sub eax, dword ptr [ecx]
        neg eax
        sbb eax, eax
        inc eax
        ret
    }
}
