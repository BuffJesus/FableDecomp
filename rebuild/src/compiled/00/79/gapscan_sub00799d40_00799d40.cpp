// Constant-return member: returns 0x90. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0x90; }