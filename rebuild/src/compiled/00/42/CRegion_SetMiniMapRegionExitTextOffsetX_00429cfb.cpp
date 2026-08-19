#pragma optimize("s",on)
// Rewrites this->p through a cdecl helper (cleaned with `pop ecx`) and returns this.
// __fastcall this=ecx, no args.
struct T { void* p; T* Fixup(); };
extern "C" void* __cdecl Helper(void* p);
T* T::Fixup() { this->p = Helper(this->p); return this; }