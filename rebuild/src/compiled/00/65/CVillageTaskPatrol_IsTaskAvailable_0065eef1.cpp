extern int __fastcall ita_probe(void*);
struct S { char pad[0x10]; char sub[4]; };
bool __fastcall IsTaskAvailable(S* self)
{
    return ita_probe(reinterpret_cast<char*>(self) + 0x10) == 0;
}