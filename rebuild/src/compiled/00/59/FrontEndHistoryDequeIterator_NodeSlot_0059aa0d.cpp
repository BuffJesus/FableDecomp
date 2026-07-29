// Front-end history deque iterator node-slot accessor @ 0x0059AA0D.
//
// A retail deque iterator is four pointers: current, first, last, and node.
// This helper returns the address of the node member, allowing the surrounding
// template machinery to update the map-node ownership slot in place.

struct FrontEndHistoryDequeIterator_0059aa0d
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C" __declspec(naked)
void**** __cdecl FrontEndHistoryDequeIterator_NodeSlot_0059aa0d(
    FrontEndHistoryDequeIterator_0059aa0d*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        add eax, 0ch
        ret
    }
}
