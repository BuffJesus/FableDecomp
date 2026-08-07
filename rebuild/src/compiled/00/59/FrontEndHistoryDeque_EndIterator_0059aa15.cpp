// Front-end history deque end-iterator accessor @ 0x0059AA15.
//
// The deque stores its begin iterator at +0 and its end iterator at +0x10.
// Each iterator is the four-pointer current/first/last/node layout recovered
// from the retail distance helper.

struct FrontEndHistoryDequeIterator_0059aa15
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

struct FrontEndHistoryDeque_0059aa15
{
    FrontEndHistoryDequeIterator_0059aa15 begin;
    FrontEndHistoryDequeIterator_0059aa15 end;
};

extern "C"
FrontEndHistoryDequeIterator_0059aa15* __cdecl
FrontEndHistoryDeque_EndIterator_0059aa15(
    FrontEndHistoryDeque_0059aa15* history)
{
    return &history->end;
}
