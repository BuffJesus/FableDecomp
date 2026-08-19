// CIVCountedPointeeBase::Delete — null-guarded virtual destroy (slot 0, flag arg 1).
// __fastcall pointer=ecx (the callee is the object's own vtable slot 0).
struct Obj {
    virtual void Destroy(int flags);
};
extern "C" void __fastcall DeleteIfSet(Obj* p) { if (p) p->Destroy(1); }
