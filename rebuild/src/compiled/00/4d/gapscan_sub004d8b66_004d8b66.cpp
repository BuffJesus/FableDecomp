// Constant-return member: returns 0xba. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xba; }