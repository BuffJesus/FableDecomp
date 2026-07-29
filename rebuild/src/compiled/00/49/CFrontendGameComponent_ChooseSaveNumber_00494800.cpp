// CFrontendGameComponent::ChooseSaveNumber @ 0x00494800.
//
// A one-based page selection resolves against the save-list vector and
// +0x1E28 first-visible index.  Valid entries are assigned to +0x1E2C before
// the component commits through virtual slot +0x10.

struct FrontEndWideString_00494800
{
    void* data;
};

class CFrontendGameComponent_00494800
{
public:
    void** vtable;
    unsigned char padding_0004[0x1e18];
    FrontEndWideString_00494800* saves_begin;
    FrontEndWideString_00494800* saves_end;
    FrontEndWideString_00494800* saves_capacity;
    unsigned long first_visible_save;
    FrontEndWideString_00494800 selected_save;

    void ChooseSaveNumber(long one_based_selection);
};

extern "C" FrontEndWideString_00494800* __fastcall
FrontEndWideStringAssign_00494800(
    FrontEndWideString_00494800* destination,
    void*,
    const FrontEndWideString_00494800* source);

__declspec(naked)
void CFrontendGameComponent_00494800::ChooseSaveNumber(long)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esi + 1e28h]
        mov edx, dword ptr [esi + 1e20h]
        lea eax, dword ptr [eax + ecx - 1]
        mov ecx, dword ptr [esi + 1e1ch]
        sub edx, ecx
        sar edx, 2
        cmp eax, edx
        jae done
        lea edx, dword ptr [ecx + eax * 4]
        push edx
        lea ecx, dword ptr [esi + 1e2ch]
        call FrontEndWideStringAssign_00494800
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 10h]

    done:
        pop esi
        ret 4
    }
}
