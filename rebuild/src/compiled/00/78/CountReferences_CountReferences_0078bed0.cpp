struct I_0078bed0 { char pad[0x148]; unsigned long f; };
struct C_0078bed0 { char pad[0x68]; I_0078bed0* p; unsigned long M(); };
unsigned long C_0078bed0::M(){ return this->p->f; }