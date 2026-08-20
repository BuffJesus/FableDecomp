// Constant-return member: returns 0x80004001. __fastcall this=ecx.
struct T { int Get(int a0, int a1); };
int T::Get(int a0, int a1) { return 0x80004001; }