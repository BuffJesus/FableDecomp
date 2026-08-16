struct S004df47b;
struct C004df47b { char pad[0x8]; char sub; void* M(); };
void* C004df47b::M(){ return &this->sub; }