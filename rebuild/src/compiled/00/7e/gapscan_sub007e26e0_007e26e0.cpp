// Constant-return member: returns 0xee. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xee; }