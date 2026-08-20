// Constant-return member: returns 0xdb. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xdb; }