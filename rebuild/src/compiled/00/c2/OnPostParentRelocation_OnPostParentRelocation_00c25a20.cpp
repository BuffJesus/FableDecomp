extern int g_vtbl_c25a20;
struct C { void* vptr; unsigned char f4; char pad[3]; int f8,fc,f10; C* OnPost(int a, int b, int c); };
C* C::OnPost(int a, int b, int c){ this->f8=c; this->vptr=&g_vtbl_c25a20; this->f4=0; this->fc=a; this->f10=b; return this; }