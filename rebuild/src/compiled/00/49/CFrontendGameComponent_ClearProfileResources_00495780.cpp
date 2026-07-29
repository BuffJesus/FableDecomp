// Recovered CFrontendGameComponent resource cleanup helper @ 0x00495780.
// The shared manifest's C3DMeshStripifier destructor label is stale.

class CFrontendProfileRange_00495780
{
public:
    void Clear(void*, void*);
};

extern void __cdecl FrontendProfileCountedDelete_00495780(void*);

class CFrontendGameComponent_00495780
{
    unsigned char m_Data[0x1e38];

public:
    void ClearProfileResources();
};

__declspec(naked)
void CFrontendGameComponent_00495780::ClearProfileResources()
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 1e08h]
        push edi
        xor edi, edi
        cmp eax, edi
        je first_counted_done
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 1e08h]
        cmp dword ptr [eax], edi
        jne first_counted_done
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov eax, dword ptr [esi + 1e08h]
        push eax
        call FrontendProfileCountedDelete_00495780
        add esp, 4

    first_counted_done:
        mov dword ptr [esi + 1e08h], edi
        mov dword ptr [esi + 1e04h], edi
        mov eax, dword ptr [esi + 1e00h]
        cmp eax, edi
        je second_counted_done
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 1e00h]
        cmp dword ptr [eax], edi
        jne second_counted_done
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov ecx, dword ptr [esi + 1e00h]
        push ecx
        call FrontendProfileCountedDelete_00495780
        add esp, 4

    second_counted_done:
        lea ecx, [esi + 1e1ch]
        mov dword ptr [esi + 1e00h], edi
        mov dword ptr [esi + 1dfch], edi
        mov edx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        push edx
        push eax
        call CFrontendProfileRange_00495780::Clear
        mov edx, dword ptr [esi + 1e34h]
        mov eax, dword ptr [esi + 1e30h]
        lea ecx, [esi + 1e30h]
        push edx
        push eax
        call CFrontendProfileRange_00495780::Clear
        pop edi
        pop esi
        ret
    }
}
