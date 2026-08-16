struct I008152a0 { char pad[0x50]; unsigned long f; };
struct C008152a0 { char pad[0x4]; I008152a0* p; unsigned long M(); };
unsigned long C008152a0::M(){ return this->p->f; }