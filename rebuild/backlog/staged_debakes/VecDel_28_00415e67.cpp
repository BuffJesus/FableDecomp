#pragma optimize("s",on)
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
struct T { void Dtor(); void* VecDel(unsigned flags); };
extern "C" void __cdecl Free1(void* p);
void* T::VecDel(unsigned flags) {
    this->Dtor();
    if (flags & 1) Free1(this);
    return this;
}
