struct I_008b0660 { char pad[0x3c]; unsigned long f; };
struct C_008b0660 { char pad[0x28]; I_008b0660* p; unsigned long M(); };
unsigned long C_008b0660::M(){ return this->p->f; }