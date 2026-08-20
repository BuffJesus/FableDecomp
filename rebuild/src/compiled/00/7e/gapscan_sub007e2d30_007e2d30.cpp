// Constant-return member: returns 0xf3. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xf3; }