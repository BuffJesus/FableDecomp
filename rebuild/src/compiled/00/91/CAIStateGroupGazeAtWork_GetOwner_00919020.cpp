void* __fastcall gb_sub_00919020(void* p);
struct P { char pad[0x18]; char* f18; };
struct S { char pad[4]; P* f4; void* M(); };
void* S::M(){ P* p=this->f4; char* q=p->f18; return gb_sub_00919020(q+368); }