// Constant-return member: returns 0xce. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xce; }