struct I_00c8f050 { char pad[0xe78]; unsigned long f; };
struct C_00c8f050 { char pad[0x1c]; I_00c8f050* p; unsigned long M(); };
unsigned long C_00c8f050::M(){ return this->p->f; }