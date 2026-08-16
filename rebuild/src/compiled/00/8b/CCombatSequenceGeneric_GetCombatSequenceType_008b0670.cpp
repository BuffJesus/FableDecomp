struct I_008b0670 { char pad[0x40]; unsigned long f; };
struct C_008b0670 { char pad[0x28]; I_008b0670* p; unsigned long M(); };
unsigned long C_008b0670::M(){ return this->p->f; }