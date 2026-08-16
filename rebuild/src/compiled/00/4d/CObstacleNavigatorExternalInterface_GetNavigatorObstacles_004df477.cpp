struct S004df477;
struct C004df477 { char pad[0x8]; char sub; void* M(); };
void* C004df477::M(){ return &this->sub; }