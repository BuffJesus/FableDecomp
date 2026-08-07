struct Sub; extern void __fastcall sub(Sub* p);
struct S { char pad[24]; void* ff; };
void __fastcall DetachTarget(S* self){ if(self->ff) sub((Sub*)((char*)self+20)); }