struct E { char pad[4]; void* fld; char pad2[0x2c-(4+4)]; };
struct S { char pad[4]; E* f4; void* M(int a0); };
void* S::M(int a0){ return this->f4[a0].fld; }