// Constant-return member: returns 0xbf. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xbf; }