// Front-end history deque iterator current-slot accessor @ 0x0059AAE0.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// Retail returns the current element slot by value.

struct FrontEndHistoryDequeIterator_0059aae0
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void** Current() const;
};

#pragma optimize("s", on)
void** FrontEndHistoryDequeIterator_0059aae0::Current() const
{
    return current;
}
#pragma optimize("s", off)
