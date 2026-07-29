// Recovered CFrontendGameComponent mode-10 profile population @ 0x00495890.
// The shared manifest's CBrushLibraryDialog::FillInListBox label is stale.

class CFrontendPopulateOps_00495890
{
public:
    void AssignEmpty(const void*);
    void ClearRange(void*, void*);
    void* ConstructRoot(const void*);
    void __fastcall CopyRoot(void*);
    void DestroyString();
    bool __fastcall StartIterator(void*, void*);
    void CopyCurrent(void*);
    void ConstructString();
    void __fastcall BuildEntry(void*, long, void*, long);
    void InsertEntry(void*, void*, void*, long, long);
    bool __fastcall NextIterator(void*, void*);
};

extern unsigned long g_FrontendPopulateCookie_00495890;
extern unsigned long g_EmptyFrontendPointer_00495890;
extern unsigned long g_FrontendProfileRoot_00495890;

extern CFrontendPopulateOps_00495890* __cdecl
GetFrontendPopulateManager_00495890();
extern void __fastcall SortFrontendProfiles_00495890(
    void*, void*, void*);
extern void __cdecl FrontendPopulateFree_00495890(void*);
extern void __fastcall CheckFrontendPopulateCookie_00495890(unsigned long);

class CFrontendGameComponent_00495890
{
    unsigned char m_Data[0x1e40];

public:
    void PopulateProfiles();
};

__declspec(naked)
void CFrontendGameComponent_00495890::PopulateProfiles()
{
    __asm
    {
        sub esp, 228h
        mov eax, dword ptr [g_FrontendPopulateCookie_00495890]
        push esi
        push edi
        mov edi, ecx
        push offset g_EmptyFrontendPointer_00495890
        lea ecx, [edi + 1e40h]
        mov dword ptr [esp + 230h], eax
        call CFrontendPopulateOps_00495890::AssignEmpty
        mov eax, dword ptr [edi + 1e34h]
        mov ecx, dword ptr [edi + 1e30h]
        lea esi, [edi + 1e30h]
        push eax
        push ecx
        mov ecx, esi
        call CFrontendPopulateOps_00495890::ClearRange
        push offset g_FrontendProfileRoot_00495890
        lea ecx, [esp + 0ch]
        call CFrontendPopulateOps_00495890::ConstructRoot
        mov edx, eax
        lea ecx, [esp + 14h]
        call CFrontendPopulateOps_00495890::CopyRoot
        lea ecx, [esp + 8]
        call CFrontendPopulateOps_00495890::DestroyString
        lea edx, [esp + 20h]
        push edx
        lea edx, [esp + 1ch]
        lea ecx, [esp + 14h]
        call CFrontendPopulateOps_00495890::StartIterator
        test al, al
        je iteration_done
        __emit 08dh
        __emit 049h
        __emit 000h

    iteration_loop:
        lea eax, [esp + 20h]
        push eax
        lea ecx, [esp + 0ch]
        call CFrontendPopulateOps_00495890::CopyCurrent
        lea ecx, [esp + 0ch]
        call CFrontendPopulateOps_00495890::ConstructString
        push 0
        lea ecx, [esp + 10h]
        push ecx
        push 0
        xor edx, edx
        lea ecx, [esp + 14h]
        call CFrontendPopulateOps_00495890::BuildEntry
        mov ecx, dword ptr [esi + 4]
        cmp ecx, dword ptr [esi + 8]
        je grow_vector
        test ecx, ecx
        je advance_end
        lea edx, [esp + 0ch]
        push edx
        call CFrontendPopulateOps_00495890::CopyCurrent

    advance_end:
        add dword ptr [esi + 4], 4
        jmp entry_inserted

    grow_vector:
        push 1
        push 1
        lea eax, [esp + 1fh]
        push eax
        lea edx, [esp + 18h]
        push edx
        push ecx
        mov ecx, esi
        call CFrontendPopulateOps_00495890::InsertEntry

    entry_inserted:
        lea ecx, [esp + 0ch]
        call CFrontendPopulateOps_00495890::DestroyString
        lea ecx, [esp + 8]
        call CFrontendPopulateOps_00495890::DestroyString
        lea eax, [esp + 20h]
        push eax
        lea edx, [esp + 1ch]
        lea ecx, [esp + 14h]
        call CFrontendPopulateOps_00495890::NextIterator
        test al, al
        jne iteration_loop

    iteration_done:
        mov ecx, dword ptr [esp + 8]
        mov edx, dword ptr [edi + 1e34h]
        push ecx
        mov ecx, dword ptr [esi]
        call SortFrontendProfiles_00495890
        lea ecx, [esp + 10h]
        mov dword ptr [edi + 1e3ch], 0
        call CFrontendPopulateOps_00495890::DestroyString
        mov ecx, dword ptr [esp + 22ch]
        pop edi
        pop esi
        call CheckFrontendPopulateCookie_00495890
        add esp, 228h
        ret
    }
}
