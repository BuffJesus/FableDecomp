// Constant-return member: returns 0xfb. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xfb; }