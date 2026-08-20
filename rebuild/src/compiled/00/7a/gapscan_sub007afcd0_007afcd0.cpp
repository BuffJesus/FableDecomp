// Constant-return member: returns 0xad. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xad; }