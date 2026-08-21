// Free pointer comparison, __stdcall (`ret 8`): returns a == b.
extern "C" bool __stdcall Compare(void* a, void* b) { return a == b; }