struct I009be210 { char pad[0x10]; unsigned long f; };
struct C009be210 { char pad[0x4]; I009be210* p; unsigned long M(); };
unsigned long C009be210::M(){ return this->p->f; }