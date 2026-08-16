struct I_008b0680 { char pad[0x44]; unsigned long f; };
struct C_008b0680 { char pad[0x28]; I_008b0680* p; unsigned long M(); };
unsigned long C_008b0680::M(){ return this->p->f; }