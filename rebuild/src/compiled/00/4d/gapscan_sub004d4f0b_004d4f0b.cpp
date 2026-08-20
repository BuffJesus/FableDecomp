// Constant-return member: returns 0xaf. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xaf; }