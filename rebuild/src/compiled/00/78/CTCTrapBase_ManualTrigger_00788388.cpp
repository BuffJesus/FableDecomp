#pragma optimize("s",on)
struct MT { char pad38[0x38]; char f38; char pad58[0x58-0x39]; char f58; void tgt(); };
void __fastcall fn_00788388(MT* self){ if(self->f58==0) return; if(self->f38!=0) return; self->tgt(); }