// Constant-return member: returns 0xaa. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xaa; }