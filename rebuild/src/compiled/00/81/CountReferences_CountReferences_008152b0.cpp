struct I008152b0 { char pad[0x54]; unsigned long f; };
struct C008152b0 { char pad[0x4]; I008152b0* p; unsigned long M(); };
unsigned long C008152b0::M(){ return this->p->f; }