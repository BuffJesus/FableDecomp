extern int g_vtbl_c2c6b0;
struct C { void* vptr; unsigned short f4,f6,f8,fa; C* Init(); };
C* C::Init(){ this->vptr=&g_vtbl_c2c6b0; this->f4=0; this->f6=0; this->f8=0; this->fa=0; return this; }