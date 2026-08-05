// Front-end history deque iterator block-begin accessor @ 0x0059AA1D.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// Retail returns the first pointer by value, identifying the start of the
// iterator's current 32-entry block.

struct FrontEndHistoryDequeIterator_0059aa1d
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void** BlockBegin() const;
};

#pragma optimize("s", on)
void** FrontEndHistoryDequeIterator_0059aa1d::BlockBegin() const
{
    return first;
}
#pragma optimize("s", off)
