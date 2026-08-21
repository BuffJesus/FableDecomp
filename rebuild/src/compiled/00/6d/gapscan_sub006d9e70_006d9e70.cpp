// Free __fastcall-free adder: `return (char*)a + 0x4;` (`ret`, cdecl).
extern "C" char* Advance(char* a) { return a + 0x4; }