// VC7.1 std::_Deque_iterator<CComponent*>::difference helper @ 0x0059A8F3.
//
// A deque block stores 32 component pointers.  Retail computes the full
// iterator distance from the node delta, finish-block prefix, and start-block
// suffix.

struct CComponent_0059a8f3;

class FrontEndDequeIterator_0059a8f3
{
public:
    CComponent_0059a8f3** current;
    CComponent_0059a8f3** first;
    CComponent_0059a8f3** last;
    CComponent_0059a8f3*** node;

    long Distance(const FrontEndDequeIterator_0059a8f3& start) const;
};

__declspec(naked)
long FrontEndDequeIterator_0059a8f3::Distance(
    const FrontEndDequeIterator_0059a8f3&) const
{
    __asm
    {
        mov edx, dword ptr [ecx + 0ch]
        mov eax, dword ptr [esp + 4]
        sub edx, dword ptr [eax + 0ch]
        push esi
        mov esi, dword ptr [ecx]
        sub esi, dword ptr [ecx + 4]
        mov ecx, dword ptr [eax + 8]
        sub ecx, dword ptr [eax]
        sar edx, 2
        sar esi, 2
        shl edx, 5
        add edx, esi
        sar ecx, 2
        lea eax, [edx + ecx - 20h]
        pop esi
        ret 4
    }
}
