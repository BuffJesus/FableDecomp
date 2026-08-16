struct I_00a47100 { char pad[0x1c]; unsigned long f; };
struct C_00a47100 { char pad[0x1c]; I_00a47100* p; unsigned long M(); };
unsigned long C_00a47100::M(){ return this->p->f; }