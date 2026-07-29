// CFrontendGameComponent::ShowMoreProfiles @ 0x004946C0.
//
// Pagination for the second frontend list advances +0x1E3C by eight when the
// +0x1E30/+0x1E34 pointer range contains more than one page.  A value strictly
// beyond the count wraps to zero; equality is retained.

class CFrontendGameComponent_004946c0
{
public:
    unsigned char padding_0000[0x1e30];
    void** entries_begin;
    void** entries_end;
    void** entries_capacity;
    unsigned long first_visible_entry;

    void ShowMoreProfiles();
};

__declspec(naked)
void CFrontendGameComponent_004946c0::ShowMoreProfiles()
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e34h]
        sub eax, dword ptr [ecx + 1e30h]
        sar eax, 2
        cmp eax, 8
        jbe done
        mov eax, dword ptr [ecx + 1e3ch]
        add eax, 8
        push esi
        mov dword ptr [ecx + 1e3ch], eax
        mov esi, dword ptr [ecx + 1e30h]
        mov edx, dword ptr [ecx + 1e34h]
        sub edx, esi
        sar edx, 2
        cmp eax, edx
        pop esi
        jbe done
        mov dword ptr [ecx + 1e3ch], 0

    done:
        ret
    }
}
