struct I008fcf90 { char pad[0x20]; unsigned long f; };
struct C008fcf90 { char pad[0x4]; I008fcf90* p; unsigned long M(); };
unsigned long C008fcf90::M(){ return this->p->f; }