// CFrontendGameComponent::GetMaxVisibleProfileNumber @ 0x00494770.
//
// This emitted save-selection list variant uses its vector at
// +0x1E1C/+0x1E20 and pagination index +0x1E28.  Remaining rows are clamped
// to eight with retail's unsigned comparison.

class CFrontendGameComponent_00494770
{
public:
    unsigned char padding_0000[0x1e1c];
    void** entries_begin;
    void** entries_end;
    void** entries_capacity;
    unsigned long first_visible_entry;

    long GetMaxVisibleProfileNumber() const;
};

__declspec(naked)
long CFrontendGameComponent_00494770::GetMaxVisibleProfileNumber() const
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e20h]
        mov edx, dword ptr [ecx + 1e28h]
        push esi
        mov esi, dword ptr [ecx + 1e1ch]
        sub eax, esi
        sar eax, 2
        sub eax, edx
        cmp eax, 8
        ja clamp_to_eight
        mov eax, dword ptr [ecx + 1e20h]
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
