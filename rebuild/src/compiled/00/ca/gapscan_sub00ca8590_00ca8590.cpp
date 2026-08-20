// Constant-return member: returns 0x80004001. __fastcall this=ecx.
struct T { int Get(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7); };
int T::Get(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7) { return 0x80004001; }