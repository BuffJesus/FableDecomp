struct P { char a[0x90]; int f90; char b[0x9c-0x94]; int f9c; char c[0xa8-0xa0]; int fa8; };
struct Sub { void MX(void* p); void MY(void* p); void MZ(void* p); };
struct C { char a[0x128]; Sub sub; void BaseUpdate(P& p, unsigned long k); void U(P& p, unsigned long k); };
void C::U(P& p, unsigned long k){ this->BaseUpdate(p,k); this->sub.MX(&p.f9c); this->sub.MY(&p.f90); this->sub.MZ(&p.fa8); }