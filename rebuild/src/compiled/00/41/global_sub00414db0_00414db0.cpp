#include "rebuild_abi.h"

/* per-element member op at 0x00589d30 (relocation-masked target).
   Called with the element-slot pointer in ecx (__fastcall this). */
extern void __fastcall elem_op_00589d30(void* elem);

/* 0x00414db0  __fastcall(ecx=first, edx=last, +1 stack arg unused); ret 0x4.
   Walks the 4-byte-element range [first, last), invoking the per-element op
   on each slot (slot pointer passed in ecx). Matches:
     push esi/edi; edi=edx; esi=ecx; cmp esi,edi; je end;
     loop: ecx=esi; call 0x589d30; esi+=4; cmp esi,edi; jne loop; end. */
void __fastcall sub_00414db0(void* ecx_first, void* edx_last, void* unused_stack)
{
    void** esi = (void**)ecx_first;
    void** edi = (void**)edx_last;
    if (esi == edi)
        return;
    do {
        elem_op_00589d30(esi);
        esi++;
    } while (esi != edi);
    (void)unused_stack;
}