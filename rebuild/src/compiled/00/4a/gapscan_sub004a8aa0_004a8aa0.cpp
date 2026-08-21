// Free __fastcall-free adder: `return (char*)a + 0xc;` (`ret`, cdecl).
extern "C" char* Advance(char* a) { return a + 0xc; }