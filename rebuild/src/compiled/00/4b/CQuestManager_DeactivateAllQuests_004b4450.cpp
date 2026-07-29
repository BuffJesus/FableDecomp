// CQuestManager::DeactivateAllQuests @ 0x004B4450.
//
// LoadGameStateInternal calls this on a normal save-state load, but skips it
// while the CWorld +0x102 hero-state latch is active.  The routine repeatedly
// snapshots the first counted quest at manager+0x38, retaining its control
// block, and asks DeactivateQuest(false) to remove it from the live list.

extern "C" void __fastcall
CQuestManager_DeactivateQuest_004b4450(
    void* manager,
    void*,
    void* quest,
    void* countedControl,
    int preserveQuest);

extern "C" __declspec(naked) void __fastcall
CQuestManager_DeactivateAllQuests_004b4450(void*, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 38h]
        cmp dword ptr [eax], eax
        je deactivate_done
        _emit 08Dh
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h

    deactivate_next:
        mov eax, dword ptr [esi + 38h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [eax + 8]
        push 0
        sub esp, 8
        mov ecx, esp
        mov dword ptr [ecx], edx
        mov eax, dword ptr [eax + 0Ch]
        test eax, eax
        mov dword ptr [ecx + 4], eax
        je counted_retained
        inc dword ptr [eax]

    counted_retained:
        mov ecx, esi
        call CQuestManager_DeactivateQuest_004b4450
        mov eax, dword ptr [esi + 38h]
        cmp dword ptr [eax], eax
        jne deactivate_next

    deactivate_done:
        pop esi
        ret
    }
}
