struct S; void __fastcall oal_sub(S* self);
struct S { char pad[0x10]; int f10; void M(); };
void S::M(){ oal_sub(this); this->f10 = 7; }