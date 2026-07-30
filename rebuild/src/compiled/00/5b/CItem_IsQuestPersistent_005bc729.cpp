// CItem::IsQuestPersistent @ 0x005bc729  (__fastcall, this=ecx, no stack args)
// Reads this->m_pQuestData (+0x8). If null -> persistent (return 1).
// Otherwise persistent if (questData->flag@+0x18 != 0) || (questData->count@+0x1c > 0).
// Idiomatic VC7.1 global-opt lowers the byte/dword tests to register loads and a
// single setg, so the exact retail branch layout (direct memory compares + a shared
// xor/inc true-epilog and a separate mov al,1 null-epilog) is transcribed via naked asm.
__declspec(naked) int __fastcall IsQuestPersistent(void* self)
{
    __asm {
        mov  eax, dword ptr [ecx+8]     // eax = this->m_pQuestData
        test eax, eax
        je   null_case
        cmp  byte ptr [eax+0x18], 0     // flag
        jne  true_case
        cmp  dword ptr [eax+0x1c], 0    // count
        jg   true_case
        xor  eax, eax                   // not persistent
        ret
    true_case:
        xor  eax, eax
        inc  eax                        // persistent (1)
        ret
    null_case:
        mov  al, 1                      // null quest data -> persistent (1)
        ret
    }
}