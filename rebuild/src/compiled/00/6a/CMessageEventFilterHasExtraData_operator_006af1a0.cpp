struct E { char pad[0x3c]; int f3c; };
struct S { int f0; int op(E* e); };
int S::op(E* e){ int d = e->f3c - this->f0; return d == 0; }