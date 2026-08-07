struct Q { char pad[0x50]; int f50; };
struct P { char pad[0x68]; Q* f68; };
struct S { char pad[0x40]; P* f40; int M(); };
int S::M(){ P* p=this->f40; Q* q=p->f68; int v=q->f50; return v != 0; }