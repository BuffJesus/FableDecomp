// Null-guarded virtual destroy (vtable slot 0, flag arg 1): the scalar-deleting
// destructor dispatch for CIVCountedPointeeBase. __fastcall pointer=ecx.
#include "engine/CIVCountedPointeeBase.h"
// The header exposes the vptr as a plain `void* __vftable`; slot 0 is a __thiscall
// virtual (this=ecx, flags on the stack), which VC7.1 can only express as a virtual
// member, so the vtable is modelled as a view struct laid over the object's vptr.
struct CIVCountedPointeeBaseVtblView { virtual void Destroy(int flags); };
extern "C" void __fastcall DeleteIfSet(CIVCountedPointeeBase* p)
{
    if (p) ((CIVCountedPointeeBaseVtblView*)(void*)&p->__vftable)->Destroy(1);
}
