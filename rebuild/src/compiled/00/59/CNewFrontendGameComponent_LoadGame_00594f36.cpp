// CNewFrontendGameComponent::LoadGame @ 0x00594F36.
//
// Retail performs virtual preparation first, marks the pending-load flag at
// +0x29, then assigns the requested wide name into the member at +0x84.

struct FrontEndWideString_00594f36
{
    void* data;
};

class CNewFrontendGameComponent_00594f36
{
public:
    void** vtable;
    unsigned char padding_004[0x25];
    unsigned char load_pending;
    unsigned char padding_02a[0x5a];
    FrontEndWideString_00594f36 load_name;

    void LoadGame(const FrontEndWideString_00594f36& name);
};

extern "C" FrontEndWideString_00594f36* __fastcall
FrontEndWideStringAssign_00594f36(
    FrontEndWideString_00594f36* destination,
    void*,
    const FrontEndWideString_00594f36* source);

__declspec(naked)
void CNewFrontendGameComponent_00594f36::LoadGame(
    const FrontEndWideString_00594f36&)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi]
        call dword ptr [eax + 10h]
        push dword ptr [esp + 8]
        lea ecx, dword ptr [esi + 84h]
        mov byte ptr [esi + 29h], 1
        call FrontEndWideStringAssign_00594f36
        pop esi
        ret 4

        // The retail oracle region includes this following field accessor.
        mov eax, dword ptr [ecx + 140h]
        ret
    }
}
