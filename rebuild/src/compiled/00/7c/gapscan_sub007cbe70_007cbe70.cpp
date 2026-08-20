// Constant-return member: returns 0xcc. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xcc; }