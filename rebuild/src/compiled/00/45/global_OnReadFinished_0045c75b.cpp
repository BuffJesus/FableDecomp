#include "rebuild_abi.h"

// OnReadFinished @ 0x0045c75b (CActiveFile), __fastcall modeled as __fastcall.
// Disasm:
//   mov eax,[ecx]      ; load this->m_ptr (+0x00)
//   test eax,eax
//   je  end            ; skip if null
//   push eax
//   call <cdecl>       ; callee(this->m_ptr)
//   pop  ecx           ; 1-arg cdecl stack cleanup
// end:
//   ret

struct CActiveFile
{
    void* m_ptr; // +0x00
};

// cdecl callee; caller pushes the arg and cleans it (pop ecx).
extern "C" void FABLE_CDECL sub_7a22b9(void* p);

void FABLE_FASTCALL OnReadFinished_0045c75b(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p)
    {
        sub_7a22b9(p);
    }
}