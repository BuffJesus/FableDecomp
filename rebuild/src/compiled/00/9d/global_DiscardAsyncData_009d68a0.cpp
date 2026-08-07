struct SubObj { void M(void* arg); };
struct P { char pad[0x28]; SubObj* f28; };
struct S { P* f0; };
void __fastcall DiscardAsyncData(S* self){ P* p = self->f0; if(p) p->f28->M(self); }