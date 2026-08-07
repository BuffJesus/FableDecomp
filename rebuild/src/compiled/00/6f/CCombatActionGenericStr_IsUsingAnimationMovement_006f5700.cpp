struct GS { char pad[0x134]; char f134; char virtual_tgt(); };
char __fastcall fn_006f5700(GS* self){ if(self->f134) return self->virtual_tgt(); return 0; }