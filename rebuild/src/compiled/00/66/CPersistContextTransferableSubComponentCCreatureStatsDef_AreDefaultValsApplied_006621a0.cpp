struct T { char pad[0x148]; unsigned char b148; };
struct S { char pad[0xe0]; T* fe0; char M(); };
char S::M(){ unsigned char v=this->fe0->b148; v>>=1; v&=1; return (char)v; }