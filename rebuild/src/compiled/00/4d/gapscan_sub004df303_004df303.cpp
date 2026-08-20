// Constant-return member: returns 0xfa. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xfa; }