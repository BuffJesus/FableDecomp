struct CThing;
struct CTCWeapon;

extern "C" bool __fastcall Helper_5dab10(CTCWeapon* self, int edx, CThing* a);

__declspec(naked) bool __fastcall IsAbleToBeUsedToBlock(CTCWeapon* self, int edx, CThing* a, CThing* b, CThing* c)
{
    __asm {
        mov eax, dword ptr [esp+4]
        push eax
        call Helper_5dab10
        test al, al
        jne  short l_true
        ret  0x0c
    l_true:
        mov al, 1
        ret  0x0c
    }
}