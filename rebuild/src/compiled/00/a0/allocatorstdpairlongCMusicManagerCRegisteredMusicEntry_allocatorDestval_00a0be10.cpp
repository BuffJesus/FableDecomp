struct S; void __fastcall dv_h1(S* p); void __fastcall dv_h2(void* p);
struct S { char pad[0xc]; int f0c; void M(); };
void S::M(){ dv_h1(this); dv_h2((char*)this + 0xc); }