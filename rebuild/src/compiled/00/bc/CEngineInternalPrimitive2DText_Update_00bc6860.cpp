struct V { int x,y,z; };
struct P { char a[0xc]; V vc; int f18,f1c,f20,f24,f28; };
struct Sub { void M(void* p); };
struct C { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; int f54,f58; char c[0x60-0x5c]; Sub sub60; int f64,f68; void U(P& p, unsigned long k); };
void C::U(P& p, unsigned long k){ this->fc=k; this->v48=p.vc; this->f54=p.f1c; this->f58=p.f20; this->f68=p.f18; this->sub60.M(&p.f24); this->f64=p.f28; }