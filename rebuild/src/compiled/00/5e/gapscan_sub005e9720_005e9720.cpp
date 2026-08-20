// Constant-return member: returns 0xfc. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xfc; }