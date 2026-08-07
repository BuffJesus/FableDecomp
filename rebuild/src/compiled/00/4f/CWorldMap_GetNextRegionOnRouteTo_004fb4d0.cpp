struct P { int gnr(int a0, int a1); };
struct S { char pad[0x44]; P* f44; int M(int a0, int a1); };
int S::M(int a0, int a1){ P* p=this->f44; if(p) return p->gnr(a0, a1); return -1; }