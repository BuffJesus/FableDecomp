// Front-end history deque iterator distance thunk @ 0x0059AAE3.
//
// This address is a tail-call alias of the canonical four-pointer iterator
// distance implementation at 0x0059A8F3.  The jump preserves ECX (`this`) and
// the stack-passed start iterator without creating another frame.

struct FrontEndHistoryDequeIterator_0059aae3
{
    void** current;
    void** first;
    void** last;
    void*** node;

    long Distance(
        const FrontEndHistoryDequeIterator_0059aae3& start) const;
};

extern "C" long __fastcall
FrontEndHistoryDequeIteratorDistance_0059aae3(
    const FrontEndHistoryDequeIterator_0059aae3* finish,
    void*,
    const FrontEndHistoryDequeIterator_0059aae3* start);

__declspec(naked)
long FrontEndHistoryDequeIterator_0059aae3::Distance(
    const FrontEndHistoryDequeIterator_0059aae3&) const
{
    __asm
    {
        jmp FrontEndHistoryDequeIteratorDistance_0059aae3
    }
}
