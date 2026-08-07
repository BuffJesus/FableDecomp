struct BW { char pad[0x14]; int f14; };
void __fastcall h1_9143c0(BW* self);
int __cdecl h2_9143c0();
void __fastcall fn_009143c0(BW* self){ h1_9143c0(self); self->f14 = h2_9143c0(); }