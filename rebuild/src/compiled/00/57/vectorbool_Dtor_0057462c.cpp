extern void __fastcall vb_helper(void*);
struct S { char pad[0x204]; int f204; };
void __fastcall Dtor(S* self){ self->f204 = 1; vb_helper(self); }