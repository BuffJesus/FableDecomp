// Constant-return member: returns 0xca. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xca; }