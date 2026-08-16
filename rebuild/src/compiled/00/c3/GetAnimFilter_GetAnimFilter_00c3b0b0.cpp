struct S00c3b0b0;
struct C00c3b0b0 { char pad[0x4]; char sub; void* M(); };
void* C00c3b0b0::M(){ return &this->sub; }