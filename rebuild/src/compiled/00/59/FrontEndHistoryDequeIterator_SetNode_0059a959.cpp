// Front-end history deque iterator node binding @ 0x0059A959.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// Binding a map node derives the 32-pointer block bounds while deliberately
// leaving current unchanged.

struct FrontEndHistoryDequeIterator_0059a959
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void SetNode(void*** new_node);
};

__declspec(naked)
void FrontEndHistoryDequeIterator_0059a959::SetNode(void***)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [ecx + 0ch], eax
        mov eax, dword ptr [eax]
        mov dword ptr [ecx + 4], eax
        add eax, 80h
        mov dword ptr [ecx + 8], eax
        ret 4
    }
}
