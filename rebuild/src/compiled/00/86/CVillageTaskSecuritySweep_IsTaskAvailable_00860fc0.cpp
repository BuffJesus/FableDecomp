extern int __fastcall ita_probe(void*);
struct S { char pad[8]; char sub[4]; };
bool __fastcall IsTaskAvailable(S* self){ return ita_probe(reinterpret_cast<char*>(self)+8) == 0; }