// Constant-return member: returns 0x100. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0x100; }