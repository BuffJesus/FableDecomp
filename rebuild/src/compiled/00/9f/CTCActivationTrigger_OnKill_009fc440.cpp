struct S;
struct P { void M(S* self); };
struct S { char pad[8]; P* f8; };
void __fastcall OnKill(S* self){ P* p=self->f8; if(p) p->M(self); }