// Constant-return member: returns 0xdf. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xdf; }