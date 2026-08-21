// Free unsigned comparison, __stdcall (`ret 8`): returns a < b.
extern "C" int __stdcall Compare(unsigned int a, unsigned int b) { return a < b; }