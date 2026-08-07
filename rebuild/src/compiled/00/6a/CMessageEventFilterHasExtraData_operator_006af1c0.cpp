struct E { char pad[8]; int f8; };
struct S { int f0; int op(E* e); };
int S::op(E* e){ int d = e->f8 - this->f0; return d == 0; }