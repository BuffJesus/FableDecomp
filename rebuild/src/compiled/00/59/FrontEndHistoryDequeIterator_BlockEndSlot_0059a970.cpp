// Front-end history deque iterator block-end slot accessor @ 0x0059A970.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// This returns the address of `last`, allowing in-place block-bound updates.

struct FrontEndHistoryDequeIterator_0059a970
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

extern "C"
void*** __cdecl FrontEndHistoryDequeIterator_BlockEndSlot_0059a970(
    FrontEndHistoryDequeIterator_0059a970* iterator)
{
    return &iterator->last;
}
