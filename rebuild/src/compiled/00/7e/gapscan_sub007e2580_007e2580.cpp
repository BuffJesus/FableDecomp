// Constant-return member: returns 0xef. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xef; }