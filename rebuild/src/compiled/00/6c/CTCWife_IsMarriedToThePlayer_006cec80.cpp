struct S; extern int __fastcall imp_helper(S*);
struct S { char pad[0x20]; int f20; };
bool __fastcall IsMarriedToThePlayer(S* self){ return self->f20 <= imp_helper(self) ? true : false; }