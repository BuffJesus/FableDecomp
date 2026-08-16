struct I_00765d90 { char pad[0x2c]; unsigned long f; };
struct C_00765d90 { char pad[0xc]; I_00765d90* p; unsigned long M(); };
unsigned long C_00765d90::M(){ return this->p->f; }