struct Sub; extern void __fastcall sub(Sub* p);
struct S { char pad[8]; void* ff; };
void __fastcall DetachTarget(S* self){ if(self->ff) sub((Sub*)((char*)self+4)); }