// Constant-return member: returns 0xec. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xec; }