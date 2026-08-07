struct OF { char pad[8]; int f8; };
void __stdcall h_b2fac0(OF* p);
int __fastcall fn_00b2fac0(void* self, int _edx, OF* arg){ h_b2fac0(arg); return arg->f8; }