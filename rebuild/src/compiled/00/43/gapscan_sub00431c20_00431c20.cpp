// Constant-return member: returns 0xbc. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xbc; }