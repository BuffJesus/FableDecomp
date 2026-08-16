struct P { char a[4]; int f4,f8,fc; unsigned char f10; };
struct C { char a[4]; int f4,f8,fc; unsigned char f10; void Set(P& p); };
void C::Set(P& p){ this->f4=p.f4; this->f8=p.f8; this->fc=p.fc; this->f10=p.f10; }