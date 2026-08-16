struct I_0051e490 { char pad[0x20]; unsigned long f; };
struct C_0051e490 { char pad[0x24]; I_0051e490* p; unsigned long M(); };
unsigned long C_0051e490::M(){ return this->p->f; }