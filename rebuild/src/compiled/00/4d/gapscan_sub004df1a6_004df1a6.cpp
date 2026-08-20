// Constant-return member: returns 0xed. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xed; }