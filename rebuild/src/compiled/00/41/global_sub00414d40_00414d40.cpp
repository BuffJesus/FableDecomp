#include "rebuild_abi.h"

/* Element callback: __fastcall on each 4-byte element (mov ecx,esi; call).
   Modeled as a free __fastcall taking the element pointer in ecx. */
extern void __fastcall elem_callback_00589da0(void* thisptr);

/* sub_00414d40: iterate [first, last) in 4-byte steps, invoking a per-element
   member call. Signature is __fastcall(first, last, <unused stack arg>);
   ret 0x4 cleans the single stack parameter. */
void __fastcall sub_00414d40(void* first, void* last, void* unused)
{
    (void)unused;
    unsigned char* esi = (unsigned char*)first;
    unsigned char* edi = (unsigned char*)last;
    if (esi == edi)
        return;
    do {
        elem_callback_00589da0((void*)esi);
        esi += 4;
    } while (esi != edi);
}