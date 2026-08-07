struct E { char pad[4]; int f4; char pad2[12-8]; };
struct S { char pad[8]; E* f8; int* M(int a0); };
int* S::M(int a0){ return &this->f8[a0].f4; }