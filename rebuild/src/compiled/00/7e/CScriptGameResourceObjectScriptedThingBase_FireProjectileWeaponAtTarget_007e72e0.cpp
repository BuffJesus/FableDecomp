struct Inner { virtual void d0();virtual void d1();virtual void d2(); virtual void V(int a0); };
struct S { char pad[8]; Inner* f8; void FireProjectileWeaponAtTarget(int a0); };
void S::FireProjectileWeaponAtTarget(int a0){ Inner* p = this->f8; if(p) p->V(a0); }