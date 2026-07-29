// CFrontendGameComponent::ShowMoreProfiles @ 0x00494560.
//
// Profile pagination advances the first-visible index by eight when the list
// has more than one page.  An index strictly beyond the profile count wraps to
// zero; equality with the count is retained by retail.

class CFrontendGameComponent_00494560
{
public:
    unsigned char padding_0000[0x1e0c];
    void** profiles_begin;
    void** profiles_end;
    void** profiles_capacity;
    unsigned long first_visible_profile;

    void ShowMoreProfiles();
};

__declspec(naked)
void CFrontendGameComponent_00494560::ShowMoreProfiles()
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e10h]
        sub eax, dword ptr [ecx + 1e0ch]
        sar eax, 2
        cmp eax, 8
        jbe done
        mov eax, dword ptr [ecx + 1e18h]
        add eax, 8
        push esi
        mov dword ptr [ecx + 1e18h], eax
        mov esi, dword ptr [ecx + 1e0ch]
        mov edx, dword ptr [ecx + 1e10h]
        sub edx, esi
        sar edx, 2
        cmp eax, edx
        pop esi
        jbe done
        mov dword ptr [ecx + 1e18h], 0

    done:
        ret
    }
}
