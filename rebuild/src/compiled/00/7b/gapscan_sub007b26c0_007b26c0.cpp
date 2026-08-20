// Constant-return member: returns 0xb1. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xb1; }