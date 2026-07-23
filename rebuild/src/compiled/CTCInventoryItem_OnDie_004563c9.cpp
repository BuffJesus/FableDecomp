extern "C" void freex(void*);
extern "C" void* newx(unsigned int);
extern "C" void basedie(void);
extern "C" void thingctor(void);
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
        push 0x70
        call newx
        test eax, eax
        pop ecx
        je short L2
        mov ecx, eax
        jmp thingctor
    L2:
        xor eax, eax
        ret
    }
}