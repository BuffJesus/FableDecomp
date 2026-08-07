struct Inner { virtual void d0();virtual void d1();virtual void d2();virtual void d3();virtual void d4();virtual void d5();virtual void d6();virtual void d7(); virtual void V(int a0); };
struct S { char pad[8]; Inner* f8; void StopFollowingThing(int a0); };
void S::StopFollowingThing(int a0){ Inner* p = this->f8; if(p) p->V(a0); }