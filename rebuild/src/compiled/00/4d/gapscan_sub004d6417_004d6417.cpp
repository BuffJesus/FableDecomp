// Constant-return member: returns 0xfe. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xfe; }