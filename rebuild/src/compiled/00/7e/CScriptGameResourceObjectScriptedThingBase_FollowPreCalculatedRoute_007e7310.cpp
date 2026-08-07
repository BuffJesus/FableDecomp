struct Inner { virtual void d0();virtual void d1();virtual void d2();virtual void d3();virtual void d4();virtual void d5(); virtual void V(int a0,int a1,int a2,int a3); };
struct S { char pad[8]; Inner* f8; void FollowPreCalculatedRoute(int a0,int a1,int a2,int a3); };
void S::FollowPreCalculatedRoute(int a0,int a1,int a2,int a3){ Inner* p = this->f8; if(p) p->V(a0,a1,a2,a3); }