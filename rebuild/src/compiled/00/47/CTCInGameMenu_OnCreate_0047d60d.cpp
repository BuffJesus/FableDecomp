// CTCInGameMenu::OnCreate @ 0x0047d60d  (__fastcall, void, 59 bytes)
//
// Retail idiom: acquire a counted-pointer subobject into a zero-initialised
// stack local via a getter (ecx = this->field4->field70, push &local), pass one
// of the returned object's fields to a this-member call, then release the
// counted pointer (--refcount at +4; if zero, virtual dtor slot 1 at [vtbl+4]).
//
// Direct-call rel32 operands and the indirect [eax+4] target are relocation-
// masked in parity, so the extern callees may carry any linkage names.

extern "C" void GetInGameMenuCountedPtr_0047d60d(void);
extern "C" void ApplyInGameMenuField_0047d60d(void);

extern "C" __declspec(naked) void __fastcall
CTCInGameMenu_OnCreate_0047d60d(void* self)
{
    (void)self;
    __asm
    {
        push ebp
        mov  ebp, esp
        push ecx
        and  dword ptr [ebp-4], 0
        push esi
        push edi
        mov  edi, ecx
        lea  eax, [ebp-4]
        push eax
        mov  eax, dword ptr [edi+4]
        mov  ecx, dword ptr [eax+70h]
        call GetInGameMenuCountedPtr_0047d60d
        mov  esi, dword ptr [ebp-4]
        push dword ptr [esi+0A8h]
        mov  ecx, edi
        call ApplyInGameMenuField_0047d60d
        dec  dword ptr [esi+4]
        jne  skip_release
        mov  eax, dword ptr [esi]
        mov  ecx, esi
        call dword ptr [eax+4]
    skip_release:
        pop  edi
        pop  esi
        leave
        ret
    }
}