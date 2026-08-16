struct S009b9870;
struct C009b9870 { char pad[0x8]; char sub; void* M(); };
void* C009b9870::M(){ return &this->sub; }