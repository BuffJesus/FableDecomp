// Constant-return member: returns 0x8000ffff. __fastcall this=ecx.
struct T { int Get(int a0, int a1); };
int T::Get(int a0, int a1) { return 0x8000ffff; }