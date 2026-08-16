struct S00c3ab20;
struct C00c3ab20 { char pad[0x4]; char sub; void* M(); };
void* C00c3ab20::M(){ return &this->sub; }