// Constant-return member: returns 0xac. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xac; }