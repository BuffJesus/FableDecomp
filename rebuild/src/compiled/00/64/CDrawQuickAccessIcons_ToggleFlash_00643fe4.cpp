#pragma optimize("s",on)
struct DF { char pad[0x3c]; int f3c; };
void __fastcall fn_00643fe4(DF* self, int _edx, int v, char b){ self->f3c = b ? v : 0; }