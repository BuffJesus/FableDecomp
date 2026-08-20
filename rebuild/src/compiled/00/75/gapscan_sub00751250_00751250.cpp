// Constant-return member: returns 0xde. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xde; }