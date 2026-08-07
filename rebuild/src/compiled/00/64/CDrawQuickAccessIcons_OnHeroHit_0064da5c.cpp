#pragma optimize("s",on)
struct OH { char pad[0x2c]; int f2c; char pad2[0x34-0x30]; char f34; };
void __fastcall h_0064da5c(OH* self);
void __fastcall fn_0064da5c(OH* self){ h_0064da5c(self); self->f2c = -1; self->f34 = 0; }