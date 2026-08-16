struct I00a3b320 { char pad[0x164]; unsigned long f; };
struct C00a3b320 { char pad[0x20]; I00a3b320* p; unsigned long M(); };
unsigned long C00a3b320::M(){ return this->p->f; }