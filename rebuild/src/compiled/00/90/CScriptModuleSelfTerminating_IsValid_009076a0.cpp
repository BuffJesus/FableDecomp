struct S; extern int __fastcall iv_helper(S*);
struct S { char pad[0x28]; int f28; };
bool __fastcall IsValid(S* self){ return iv_helper(self) > self->f28 ? true : false; }