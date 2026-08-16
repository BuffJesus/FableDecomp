struct V { int x,y,z; };
struct P { char a[0xc]; unsigned long fc; V v10; unsigned long f1c,f20,f24,f28; };
struct C { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; unsigned long f54,f58,f5c,f60,f64; void U(P const& p, unsigned long k); };
void C::U(P const& p, unsigned long k){ this->fc=k; this->v48=p.v10; this->f54=p.f1c; this->f58=p.f20; this->f5c=p.f24; this->f60=p.f28; this->f64=p.fc; }