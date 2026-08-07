struct RM { void* vptr; char pad[0x44]; unsigned int f48; void dtor(); };
void __fastcall fn_00c3d7b0(RM* self){ self->vptr=(void*)0x12b5a80; self->f48=0x80000000u; self->dtor(); }