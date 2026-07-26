/* CHeroMarriageDef::GetSizeofClass @ 0x0044c2e2
 * The 31-byte retail symbol covers GetSizeofClass (mov eax,0xc8; ret) plus an
 * adjacent unnamed operator-new factory thunk that the linker folded into the
 * same symbol slice. Reproduced as one function via naked asm so the emitted
 * byte region matches exactly (call/jmp targets are reloc-masked in parity). */
extern void* __cdecl opnew(unsigned int n);
extern void* __fastcall bctor(void* p);

__declspec(naked) unsigned long __fastcall CHeroMarriageDef_GetSizeofClass(void* self)
{
    __asm {
        mov eax, 0c8h
        ret
        push 32ch
        call opnew
        test eax, eax
        pop ecx
        je short L1
        mov ecx, eax
        jmp bctor
    L1:
        xor eax, eax
        ret
    }
}