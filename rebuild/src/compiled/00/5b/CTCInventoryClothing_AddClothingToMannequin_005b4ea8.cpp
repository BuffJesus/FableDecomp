#pragma optimize("s",on)
struct IC { char pad[0x158]; int f158; };
int __stdcall h_005b4ea8(int field, int arg);
int __fastcall fn_005b4ea8(IC* self, int _edx, int arg){ return h_005b4ea8(arg, self->f158); }