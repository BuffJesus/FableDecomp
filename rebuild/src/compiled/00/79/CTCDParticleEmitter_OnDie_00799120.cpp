struct S; extern S* __fastcall od1(S* self); extern void __fastcall od2(S* p);
struct S { int x; void M(); };
void S::M(){ S* p=od1(this); if(p) od2(p); }