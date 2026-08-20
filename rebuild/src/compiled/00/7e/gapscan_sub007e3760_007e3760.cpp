// Constant-return member: returns 0xff. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xff; }