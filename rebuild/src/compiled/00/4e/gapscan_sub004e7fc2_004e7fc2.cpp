// Constant-return member: returns 0xcd. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xcd; }