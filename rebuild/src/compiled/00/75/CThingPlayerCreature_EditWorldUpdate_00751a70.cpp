struct Sub; extern unsigned char __fastcall ewu_probe(Sub*);
struct S { char pad[4]; Sub* sub; };
bool __fastcall EditWorldUpdate(S* self){ return ewu_probe(self->sub) ? true : false; }