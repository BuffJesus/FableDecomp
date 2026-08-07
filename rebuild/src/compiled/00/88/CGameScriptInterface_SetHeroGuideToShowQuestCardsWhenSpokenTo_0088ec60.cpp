struct Inner { char pad[0xda]; unsigned char b_da; };
struct S { char pad[4]; Inner* f4; void Set(unsigned char v); };
void S::Set(unsigned char v){ this->f4->b_da = v; }