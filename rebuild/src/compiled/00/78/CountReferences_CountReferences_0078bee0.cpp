struct I_0078bee0 { char pad[0x14c]; unsigned long f; };
struct C_0078bee0 { char pad[0x68]; I_0078bee0* p; unsigned long M(); };
unsigned long C_0078bee0::M(){ return this->p->f; }