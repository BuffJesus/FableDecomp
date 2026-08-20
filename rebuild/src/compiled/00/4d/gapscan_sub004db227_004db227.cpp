// Constant-return member: returns 0xd1. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xd1; }