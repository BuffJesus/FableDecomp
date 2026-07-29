// CFrontendGameComponent::GetMaxVisibleProfileNumber @ 0x00494520.
//
// The older initial-frontend component stores a vector of profile pointers at
// +0x1E0C/+0x1E10 and the first visible profile index at +0x1E18.  Retail
// returns min(profile_count - first_visible, 8) using an unsigned clamp.

class CFrontendGameComponent_00494520
{
public:
    unsigned char padding_0000[0x1e0c];
    void** profiles_begin;
    void** profiles_end;
    void** profiles_capacity;
    unsigned long first_visible_profile;

    long GetMaxVisibleProfileNumber() const;
};

__declspec(naked)
long CFrontendGameComponent_00494520::GetMaxVisibleProfileNumber() const
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e10h]
        mov edx, dword ptr [ecx + 1e18h]
        push esi
        mov esi, dword ptr [ecx + 1e0ch]
        sub eax, esi
        sar eax, 2
        sub eax, edx
        cmp eax, 8
        ja clamp_to_eight
        mov eax, dword ptr [ecx + 1e10h]
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
