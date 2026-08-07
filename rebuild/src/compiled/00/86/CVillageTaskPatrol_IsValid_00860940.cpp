extern int __fastcall probe(void*);
struct S { char pad2[0x40]; };
bool __fastcall IsValid(S* self){ return probe(reinterpret_cast<char*>(self)+8) != 0; }