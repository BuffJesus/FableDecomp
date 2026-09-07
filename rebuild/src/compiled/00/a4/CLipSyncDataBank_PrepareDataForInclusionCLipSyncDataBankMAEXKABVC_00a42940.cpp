// Byte-exact reconstruction of
// CLipSyncDataBank::PrepareDataForInclusion @ 0x00a42940
//
// Disasm shows the interesting work is a CCountedPointer<CBankStateBlock>
// release: the pointer arrives by value on the stack (last param), and the
// body decrements its embedded refcount; on reaching zero it invokes the
// virtual destructor via [esi+4] (ecx=[esi+8]) then frees the block via a
// dedicated deallocator at 0x00bfe9bc. ret 0x1c pops 28 bytes of args.

#include "engine/CLipSyncDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap;
struct CArrayCCharString;
struct CDataOutputStream;

// The refcounted control block. Layout implied by the disasm:
//   +0x00 : long refCount           (dec [esi])
//   +0x04 : void (__fastcall *dtor)(void* this)   (call [esi+4])
//   +0x08 : void* object            (mov ecx,[esi+8])
struct CBankStateBlockRef {
    long  refCount;
    void  (__fastcall *dtor)(void* self);
    void* object;
};

// The dedicated deallocator (operator delete-ish) at 0x00bfe9bc.
extern "C" void __cdecl BankStateBlockFree(CBankStateBlockRef* p);


// __fastcall member modelled as __fastcall(self, edxDummy, args...).
// Params (28 bytes = 7 dwords):
//   this(ecx) , dummy(edx) , map, key, /*counted ptr by value*/ ref,
//   dataOut, /*extra 3 dwords consumed by the CArray/CCountedPointer
//   pass-by-value ABI so ret 0x1c balances*/.
//
// Only the counted-pointer argument is touched: it sits at [esp+0x18] on
// entry (after the single push esi), i.e. the 5th dword argument.
void __fastcall CLipSyncDataBank_PrepareDataForInclusion(
        CLipSyncDataBank* self, int edxDummy,
        CWorldMap* map, long key,
        CDataOutputStream* d3, int a4,
        CBankStateBlockRef* ref,
        int a6, int a7)
{
    (void)self; (void)edxDummy; (void)map; (void)key;
    (void)d3; (void)a4; (void)a6; (void)a7;

    if (ref) {
        if (--ref->refCount == 0) {
            ref->dtor(ref->object);
            BankStateBlockFree(ref);
        }
    }
}