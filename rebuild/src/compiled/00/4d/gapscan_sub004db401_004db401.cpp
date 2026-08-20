// Constant-return member: returns 0xf0. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xf0; }