// __stdcall free function: clear the byte pointed to by the SECOND argument.
extern "C" void __stdcall Clear(int unused, char* p) { *p = 0; }