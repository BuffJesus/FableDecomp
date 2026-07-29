// Front-end history deque end-iterator accessor @ 0x0059A985.
//
// The deque contains two consecutive 16-byte iterator records: begin at +0
// and end at +0x10.  This emitted helper returns the end record by address.

struct FrontEndHistoryDequeIterator_0059a985
{
    void** current;
    void** first;
    void** last;
    void*** node;
};

struct FrontEndHistoryDeque_0059a985
{
    FrontEndHistoryDequeIterator_0059a985 begin;
    FrontEndHistoryDequeIterator_0059a985 end;
};

extern "C" __declspec(naked)
FrontEndHistoryDequeIterator_0059a985* __cdecl
FrontEndHistoryDeque_EndIterator_0059a985(
    FrontEndHistoryDeque_0059a985*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        add eax, 10h
        ret
    }
}
