// Byte-exact reconstruction of
// ?PrepareDataForInclusion@CShaderDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z
// this-call member with a by-value CCountedPointer arg; body only touches that arg's control block.

#include "engine/CShaderDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap;
struct CArrayCS;
struct CDataOutputStream;

// Reference-count control block held by a CCountedPointer<CBankStateBlock>.
struct RefBlock {
    long        refs;    // +0x00
    void      (__fastcall *dtor)(void* self); // +0x04  (called with ecx = target)
    void*       target;  // +0x08
};

// cdecl deleter (operator delete style free)
extern "C" void __cdecl free_block(void* p);


// The by-value CCountedPointer arg lands as a stack dword = pointer to RefBlock.
// __fastcall: self->ecx, edxDummy->edx (consumed from stack model), rest on stack.
// Need 7 stack dwords total for ret 0x1c, esi = 5th stack dword ([esp+0x18] after push esi).
void __fastcall PrepareDataForInclusion(
        CShaderDataBank* self,     // ecx
        void* edxDummy,            // edx
        long s1,                   // stack [esp+8]
        void* s2,                  // stack [esp+0xC]
        void* s3,                  // stack [esp+0x10]
        void* s4,                  // stack [esp+0x14]
        RefBlock* ptr,             // stack [esp+0x18] <- esi (5th stack dword)
        void* s6,                  // stack [esp+0x1c]
        void* s7)                  // stack [esp+0x20]  -> 7 stack dwords -> ret 0x1c
{
    (void)self; (void)edxDummy; (void)s1; (void)s2; (void)s3; (void)s4; (void)s6; (void)s7;
    if (ptr) {
        if (--ptr->refs == 0) {
            ptr->dtor(ptr->target);
            free_block(ptr);
        }
    }
}