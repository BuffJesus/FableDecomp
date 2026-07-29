// Recovered CFrontendGameComponent destructor body @ 0x00495620.
// The shared manifest's NHeroInformationScreens label is stale.

class CFrontendCleanupObject_00495620
{
public:
    void DestroySubobject();
    void DestroyString();
    void DestroyBase();
};

extern unsigned long g_CFrontendDerivedVTable0_00495620;
extern unsigned long g_CFrontendDerivedVTable4_00495620;
extern unsigned long g_CFrontendBaseVTable0_00495620;
extern unsigned long g_CFrontendBaseVTable4_00495620;

extern void __cdecl FrontendVectorFree_00495620(void*);
extern void __cdecl FrontendCountedDelete_00495620(void*);

class CFrontendGameComponent_00495620
{
    unsigned char m_Data[0x1e5c];

public:
    void Destroy();
};

__declspec(naked) void CFrontendGameComponent_00495620::Destroy()
{
    __asm
    {
        push ebx
        push ebp
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esi + 1e58h]
        mov dword ptr [esi], offset g_CFrontendDerivedVTable0_00495620
        mov dword ptr [esi + 4], offset g_CFrontendDerivedVTable4_00495620
        call CFrontendCleanupObject_00495620::DestroySubobject
        lea ecx, [esi + 1e40h]
        call CFrontendCleanupObject_00495620::DestroyString
        mov ebx, dword ptr [esi + 1e34h]
        mov edi, dword ptr [esi + 1e30h]
        cmp edi, ebx
        je first_vector_done
        __emit 08dh
        __emit 0a4h
        __emit 024h
        __emit 000h
        __emit 000h
        __emit 000h
        __emit 000h

    first_vector_loop:
        mov ecx, edi
        call CFrontendCleanupObject_00495620::DestroyString
        add edi, 4
        cmp edi, ebx
        jne first_vector_loop

    first_vector_done:
        mov eax, dword ptr [esi + 1e30h]
        xor ebp, ebp
        cmp eax, ebp
        je first_allocation_done
        push eax
        call FrontendVectorFree_00495620
        add esp, 4

    first_allocation_done:
        lea ecx, [esi + 1e2ch]
        call CFrontendCleanupObject_00495620::DestroyString
        mov ebx, dword ptr [esi + 1e20h]
        mov edi, dword ptr [esi + 1e1ch]
        cmp edi, ebx
        je second_vector_done
        mov edi, edi

    second_vector_loop:
        mov ecx, edi
        call CFrontendCleanupObject_00495620::DestroyString
        add edi, 4
        cmp edi, ebx
        jne second_vector_loop

    second_vector_done:
        mov eax, dword ptr [esi + 1e1ch]
        cmp eax, ebp
        je second_allocation_done
        push eax
        call FrontendVectorFree_00495620
        add esp, 4

    second_allocation_done:
        mov ebx, dword ptr [esi + 1e10h]
        mov edi, dword ptr [esi + 1e0ch]
        cmp edi, ebx
        je third_vector_done

    third_vector_loop:
        mov ecx, edi
        call CFrontendCleanupObject_00495620::DestroyString
        add edi, 4
        cmp edi, ebx
        jne third_vector_loop

    third_vector_done:
        mov eax, dword ptr [esi + 1e0ch]
        cmp eax, ebp
        je third_allocation_done
        push eax
        call FrontendVectorFree_00495620
        add esp, 4

    third_allocation_done:
        mov eax, dword ptr [esi + 1e08h]
        cmp eax, ebp
        je first_counted_done
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 1e08h]
        cmp dword ptr [eax], ebp
        jne first_counted_done
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov eax, dword ptr [esi + 1e08h]
        push eax
        call FrontendCountedDelete_00495620
        add esp, 4

    first_counted_done:
        mov dword ptr [esi + 1e04h], ebp
        mov dword ptr [esi + 1e08h], ebp
        mov eax, dword ptr [esi + 1e00h]
        cmp eax, ebp
        je second_counted_done
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 1e00h]
        cmp dword ptr [eax], ebp
        jne second_counted_done
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov ecx, dword ptr [esi + 1e00h]
        push ecx
        call FrontendCountedDelete_00495620
        add esp, 4

    second_counted_done:
        mov dword ptr [esi + 1dfch], ebp
        mov dword ptr [esi + 1e00h], ebp
        pop edi
        mov dword ptr [esi], offset g_CFrontendBaseVTable0_00495620
        mov dword ptr [esi + 4], offset g_CFrontendBaseVTable4_00495620
        mov ecx, esi
        pop esi
        pop ebp
        pop ebx
        jmp CFrontendCleanupObject_00495620::DestroyBase
    }
}
