struct V { int x,y,z; };
struct P { char a[0xc]; V vc; V v18; unsigned long f24,f28; };
struct C { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; V v54; unsigned long f60,f64; void U(P const& p, unsigned long k); };
void C::U(P const& p, unsigned long k){ this->fc=k; this->v48=p.vc; this->v54=p.v18; this->f60=p.f28; this->f64=p.f24; }