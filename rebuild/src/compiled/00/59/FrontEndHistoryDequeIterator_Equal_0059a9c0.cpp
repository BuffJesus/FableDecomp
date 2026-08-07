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

extern "C"
bool __cdecl FrontEndHistoryDequeIterator_Equal_0059a9c0(
    const FrontEndHistoryDequeIterator_0059a9c0* left,
    const FrontEndHistoryDequeIterator_0059a9c0* right)
{
    return left->current == right->current;
}
