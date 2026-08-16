struct I_0079a610 { char pad[0x54]; unsigned long f; };
struct C_0079a610 { char pad[0xc]; I_0079a610* p; unsigned long M(); };
unsigned long C_0079a610::M(){ return this->p->f; }