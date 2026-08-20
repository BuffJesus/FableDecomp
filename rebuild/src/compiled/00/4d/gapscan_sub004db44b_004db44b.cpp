// Constant-return member: returns 0xf1. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xf1; }