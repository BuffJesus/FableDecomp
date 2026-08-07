struct S; int __fastcall ss_sub(S* self);
struct S { int x; void M(); };
void S::M(){ int r; do { r = ss_sub(this); } while(r==4); }