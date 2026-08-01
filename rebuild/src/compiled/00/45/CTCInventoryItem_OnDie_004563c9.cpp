extern "C" void freex(void*);
extern "C" void basedie(void);
extern "C" void* g_vtbl;

__declspec(naked) void __fastcall CTCInventoryItem_OnDie(void* self)
{
    __asm {
        push esi
        mov esi, ecx
        mov eax, [esi+0x28]
        test eax, eax
        je short L1
        push eax
        call freex
        pop ecx
    L1:
        mov dword ptr [esi], offset g_vtbl
        mov ecx, esi
        pop esi
        jmp basedie
    }
}
