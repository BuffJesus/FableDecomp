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

extern "C"
void**** __cdecl FrontEndHistoryDequeIterator_NodeSlot_0059aa0d(
    FrontEndHistoryDequeIterator_0059aa0d* iterator)
{
    return &iterator->node;
}
