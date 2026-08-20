// Constant-return member: returns 0x45. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0x45; }