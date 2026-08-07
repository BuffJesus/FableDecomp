extern int __fastcall probe(void*);
struct S { char pad2[0x100]; };
bool __fastcall f(S* self){ return probe(reinterpret_cast<char*>(self)+0x10) != 0; }