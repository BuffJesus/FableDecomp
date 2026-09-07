// Byte-exact reconstruction of
// CMeshDataBank_Methods::PrepareDataForInclusion (retail 0x00a24410)
//
// Retail body = inlined destruction of a by-value
// CCountedPointer<CBankStateBlock> stack argument: decrement the shared
// refcount block; when it hits zero, invoke the block's virtual dtor
// (this in ecx, no args) and free the block. `this` (the CMeshDataBank)
// is never touched, so retail emits a frameless esi-based body.

#include "engine/CMeshDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct RefBlock {
    long          count;   // +0x00
    void        (__fastcall *dtor)(void* self); // +0x04 vfn slot (thiscall)
    void*         obj;      // +0x08
};

extern "C" void __cdecl EngineFree(void* p);   // retail 0x00bfe9bc

struct CWorldMap;
struct CArrayStr;
struct CCountedPtr { RefBlock* blk; };
struct CDataOutputStream;

// Real member -> genuine __fastcall (this in ecx, homed only if used).
struct CMeshDataBank_Methods : CMeshDataBank {
    void PrepareDataForInclusion(
        CWorldMap*  a,           // +0x04
        long        b,           // +0x08
        CArrayStr*  c,           // +0x0c
        long        d,           // +0x10
        CCountedPtr sp,          // +0x14 by-value counted pointer
        CDataOutputStream* e,    // +0x18
        CDataOutputStream* f);
    // +0x1c (pads ret 0x1c);
};

void CMeshDataBank_Methods::PrepareDataForInclusion(
    CWorldMap* a, long b, CArrayStr* c, long d,
    CCountedPtr sp, CDataOutputStream* e, CDataOutputStream* f)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    RefBlock* blk = sp.blk;
    if (blk) {
        if (--blk->count == 0) {
            blk->dtor(blk->obj);
            EngineFree(blk);
        }
    }
}