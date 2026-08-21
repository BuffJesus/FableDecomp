#pragma optimize("s",on)
// Construct-and-return helper: run a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall Construct(T* p) { p->Init(); return p; }