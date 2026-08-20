// Constant-return member: returns 0xc3. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xc3; }