// CFrontendGameComponent::ChooseWorldNumber @ 0x00494710.
//
// Selection is one-based within the current page.  A valid entry is suffixed
// with ".wld" into a temporary, copied to +0x1E40, destroyed, then committed
// through virtual slot +0x10.

struct FrontEndWideString_00494710
{
    void* data;
};

class CFrontendGameComponent_00494710
{
public:
    void** vtable;
    unsigned char padding_0004[0x1e2c];
    FrontEndWideString_00494710* entries_begin;
    FrontEndWideString_00494710* entries_end;
    FrontEndWideString_00494710* entries_capacity;
    unsigned long first_visible_entry;
    FrontEndWideString_00494710 selected_world;

    void ChooseWorldNumber(long one_based_selection);
};

extern "C" unsigned short FrontEndWorldSuffix_00494710[];

extern "C" FrontEndWideString_00494710* __fastcall
FrontEndWideStringConcat_00494710(
    FrontEndWideString_00494710* result,
    const FrontEndWideString_00494710* source,
    const unsigned short* suffix);

extern "C" FrontEndWideString_00494710* __fastcall
FrontEndWideStringAssign_00494710(
    FrontEndWideString_00494710* destination,
    void*,
    const FrontEndWideString_00494710* source);

extern "C" void __fastcall
FrontEndWideStringDestroy_00494710(
    FrontEndWideString_00494710* value,
    void*);

__declspec(naked)
void CFrontendGameComponent_00494710::ChooseWorldNumber(long)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esi + 1e3ch]
        mov edx, dword ptr [esi + 1e34h]
        lea eax, dword ptr [eax + ecx - 1]
        mov ecx, dword ptr [esi + 1e30h]
        sub edx, ecx
        sar edx, 2
        cmp eax, edx
        jae done
        lea edx, dword ptr [ecx + eax * 4]
        push offset FrontEndWorldSuffix_00494710
        lea ecx, dword ptr [esp + 0ch]
        call FrontEndWideStringConcat_00494710
        push eax
        lea ecx, dword ptr [esi + 1e40h]
        call FrontEndWideStringAssign_00494710
        lea ecx, dword ptr [esp + 8]
        call FrontEndWideStringDestroy_00494710
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 10h]

    done:
        pop esi
        ret 4
    }
}
