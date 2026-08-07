struct Inner { virtual void d0();virtual void d1();virtual void d2();virtual void d3();virtual void d4();virtual void d5();virtual void d6(); virtual void V(int a0,int a1,int a2); };
struct S { char pad[8]; Inner* f8; void FollowThing(int a0,int a1,int a2); };
void S::FollowThing(int a0,int a1,int a2){ Inner* p = this->f8; if(p) p->V(a0,a1,a2); }