extern int __fastcall isv_probe(void*);
struct S { char pad[0x14]; char sub[4]; };
bool __fastcall IsStillValid(S* self){ return isv_probe(reinterpret_cast<char*>(self)+0x14) ? true : false; }