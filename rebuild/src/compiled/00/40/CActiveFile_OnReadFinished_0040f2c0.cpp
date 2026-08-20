// `if (this->p) Free1(this->p);` with `pop ecx` cleanup. __fastcall this=ecx.
#pragma pack(push,1)
struct T { void* p; void OnReadFinished(); };
#pragma pack(pop)
extern "C" void __cdecl Free1(void* p);
void T::OnReadFinished() { if (this->p) Free1(this->p); }