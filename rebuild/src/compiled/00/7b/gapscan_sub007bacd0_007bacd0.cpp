// Constant-return member: returns 0xab. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xab; }