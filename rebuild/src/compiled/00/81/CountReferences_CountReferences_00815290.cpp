struct I00815290 { char pad[0x48]; unsigned long f; };
struct C00815290 { char pad[0x4]; I00815290* p; unsigned long M(); };
unsigned long C00815290::M(){ return this->p->f; }