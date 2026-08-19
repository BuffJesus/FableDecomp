// Constant-null one-argument getter: `xor eax,eax; ret 4`.
struct T { void* GetPSubDefInfo(int index); };
void* T::GetPSubDefInfo(int index) { return 0; }
