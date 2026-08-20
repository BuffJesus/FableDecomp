// Constant-return member: returns 0xdd. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xdd; }