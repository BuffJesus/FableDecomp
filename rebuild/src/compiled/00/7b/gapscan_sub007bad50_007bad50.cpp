// Constant-return member: returns 0xbd. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xbd; }