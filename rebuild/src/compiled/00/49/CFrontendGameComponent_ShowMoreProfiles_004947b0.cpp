// CFrontendGameComponent::ShowMoreProfiles @ 0x004947B0.
//
// Save-list pagination uses the +0x1E1C/+0x1E20 vector and advances the
// +0x1E28 first-visible index by eight.  Values strictly past the count wrap
// to zero, while equality is retained.

class CFrontendGameComponent_004947b0
{
public:
    unsigned char padding_0000[0x1e1c];
    void** entries_begin;
    void** entries_end;
    void** entries_capacity;
    unsigned long first_visible_entry;

    void ShowMoreProfiles();
};

__declspec(naked)
void CFrontendGameComponent_004947b0::ShowMoreProfiles()
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e20h]
        sub eax, dword ptr [ecx + 1e1ch]
        sar eax, 2
        cmp eax, 8
        jbe done
        mov eax, dword ptr [ecx + 1e28h]
        add eax, 8
        push esi
        mov dword ptr [ecx + 1e28h], eax
        mov esi, dword ptr [ecx + 1e1ch]
        mov edx, dword ptr [ecx + 1e20h]
        sub edx, esi
        sar edx, 2
        cmp eax, edx
        pop esi
        jbe done
        mov dword ptr [ecx + 1e28h], 0

    done:
        ret
    }
}
