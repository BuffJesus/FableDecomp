struct T { char pad[0x48]; int f48; char pad2[0x64-0x4c]; unsigned char b64; };
struct S { char pad[0x28]; T* f28; char M(); };
char S::M(){ T* p=this->f28; if(p->f48) return (char)p->b64; return 0; }