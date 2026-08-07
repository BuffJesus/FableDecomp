struct That { void m(int id); };
struct CM { char pad[0x4]; That* f4; char pad2[0x54-0x8]; char f54; };
void __fastcall fn_0079c640(CM* self){ self->f54 = 0; self->f4->m(0x92); }