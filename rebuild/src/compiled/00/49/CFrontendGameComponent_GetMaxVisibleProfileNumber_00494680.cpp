// CFrontendGameComponent::GetMaxVisibleProfileNumber @ 0x00494680.
//
// This second emitted frontend-list variant uses the pointer vector at
// +0x1E30/+0x1E34 and pagination index +0x1E3C.  Retail returns the unsigned
// clamped remaining row count, with a maximum of eight.

class CFrontendGameComponent_00494680
{
public:
    unsigned char padding_0000[0x1e30];
    void** entries_begin;
    void** entries_end;
    void** entries_capacity;
    unsigned long first_visible_entry;

    long GetMaxVisibleProfileNumber() const;
};

__declspec(naked)
long CFrontendGameComponent_00494680::GetMaxVisibleProfileNumber() const
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e34h]
        mov edx, dword ptr [ecx + 1e3ch]
        push esi
        mov esi, dword ptr [ecx + 1e30h]
        sub eax, esi
        sar eax, 2
        sub eax, edx
        cmp eax, 8
        ja clamp_to_eight
        mov eax, dword ptr [ecx + 1e34h]
        sub eax, esi
        sar eax, 2
        sub eax, edx
        pop esi
        ret

    clamp_to_eight:
        mov eax, 8
        pop esi
        ret
    }
}
