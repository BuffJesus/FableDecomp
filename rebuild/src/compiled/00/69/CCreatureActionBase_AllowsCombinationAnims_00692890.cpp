struct S; extern int __fastcall sub(void* p);
struct S { char pad[0x74]; void* f74; char M(); };
char S::M(){ unsigned int r=(unsigned int)sub(this->f74); return (char)((~(r>>3))&1); }