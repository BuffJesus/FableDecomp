struct N { void h(int a, int b); };
struct BL { char pad[8]; N* f8; };
void __fastcall fn_00a35b30(BL* self, int _edx, int arg){ self->f8->h(arg, 0); }