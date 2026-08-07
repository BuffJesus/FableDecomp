struct Sub; extern unsigned int __fastcall probe(Sub*);
struct S { char pad[0x74]; Sub* f74; };
bool __fastcall OverrideFaceWithDefault(S* self){ return (probe(self->f74) >> 4) & 1; }