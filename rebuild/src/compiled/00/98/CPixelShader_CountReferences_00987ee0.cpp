struct I00987ee0 { char pad[0x8]; unsigned long f; };
struct C00987ee0 { char pad[0x4]; I00987ee0* p; unsigned long M(); };
unsigned long C00987ee0::M(){ return this->p->f; }