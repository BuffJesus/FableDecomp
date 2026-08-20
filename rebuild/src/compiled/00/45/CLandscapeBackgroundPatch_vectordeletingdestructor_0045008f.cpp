#pragma optimize("s",on)
// vector deleting destructor that first re-seats the vptr (the compiler-generated
// destructor prologue for a polymorphic class), then destroys and optionally frees.
// __fastcall this=ecx, flags=stack (ret 4).
extern void* g_vtable[];              // 0x01230BA0
struct T { void** vptr; void Dtor(); void* VecDel(unsigned flags); };
extern "C" void __cdecl Free1(void* p);
void* T::VecDel(unsigned flags) {
    this->vptr = g_vtable;
    this->Dtor();
    if (flags & 1) Free1(this);
    return this;
}