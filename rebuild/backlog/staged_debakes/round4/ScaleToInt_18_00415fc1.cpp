// `return (int)(g_scale * value);` — fild the global, fmul the stack float, __ftol.
// __fastcall this=ecx (unused), value=stack (ret 4).
extern int g_scale;          // 0x01375550
struct T { int Scale(float value); };
int T::Scale(float value) { return (int)(g_scale * value); }
