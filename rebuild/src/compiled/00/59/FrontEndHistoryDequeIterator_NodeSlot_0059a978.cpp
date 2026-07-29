// Front-end history deque iterator node-slot accessor @ 0x0059A978.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// This emitted template instance returns the address of the map-node member.

struct FrontEndHistoryDequeIterator_0059a978
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C" __declspec(naked)
void**** __cdecl FrontEndHistoryDequeIterator_NodeSlot_0059a978(
    FrontEndHistoryDequeIterator_0059a978*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        add eax, 0ch
        ret
    }
}
