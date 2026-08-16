struct I_007fde60 { char pad[0x38]; unsigned long f; };
struct C_007fde60 { char pad[0xc]; I_007fde60* p; unsigned long M(); };
unsigned long C_007fde60::M(){ return this->p->f; }