struct I008fcf80 { char pad[0x20]; unsigned long f; };
struct C008fcf80 { char pad[0x4]; I008fcf80* p; unsigned long M(); };
unsigned long C008fcf80::M(){ return this->p->f; }