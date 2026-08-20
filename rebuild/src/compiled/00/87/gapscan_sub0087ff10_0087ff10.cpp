// Constant-return member: returns 0xd. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xd; }