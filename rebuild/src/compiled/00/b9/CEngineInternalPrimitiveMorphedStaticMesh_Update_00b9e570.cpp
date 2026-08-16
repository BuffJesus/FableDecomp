struct P { char a[0x4c]; int f4c; char b[0x58-0x50]; int f58; };
struct Sub { void MX(void* p); void MY(void* p); };
struct C { char a[0xc]; unsigned long fc; char b[0xac-0x10]; Sub sub; void BaseUpdate(P& p, unsigned long k); void U(P& p, unsigned long k); };
void C::U(P& p, unsigned long k){ this->fc=k; this->BaseUpdate(p,k); this->sub.MX(&p.f4c); this->sub.MY(&p.f58); }