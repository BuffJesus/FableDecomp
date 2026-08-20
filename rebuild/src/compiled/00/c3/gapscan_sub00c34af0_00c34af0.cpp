// Constant-return member: returns 0x20100. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0x20100; }