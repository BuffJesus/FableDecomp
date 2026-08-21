struct E { int f0; void* f4; };
struct C_00bc55b0 { char pad[0x48]; E* b; E* e; void* M(unsigned long i); };
void* C_00bc55b0::M(unsigned long i){ if((long)i < this->e-this->b){ void* p=this->b[i].f4; if(p) return p; } return 0; }