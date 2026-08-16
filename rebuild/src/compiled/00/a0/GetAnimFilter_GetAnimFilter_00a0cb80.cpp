struct S00a0cb80;
struct C00a0cb80 { char pad[0x8]; char sub; void* M(); };
void* C00a0cb80::M(){ return &this->sub; }