struct DV { char pad[4]; char f4; void m(); };
void __fastcall fn_004a9140(DV* self){ ((DV*)((char*)self+4))->m(); self->m(); }