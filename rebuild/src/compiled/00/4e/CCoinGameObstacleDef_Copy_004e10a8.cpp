struct CDefClassBase;

struct CDefBase {
    void Inherit(CDefClassBase const *src);   // __fastcall: this in ecx, src on stack
};

struct CCoinGameObstacleDef {
    char pad[0x28];
    unsigned long f28;
    void __fastcall Copy(int /*edx*/, CDefClassBase const *src);
};

void __fastcall CCoinGameObstacleDef::Copy(int /*edx*/, CDefClassBase const *src)
{
    ((CDefBase *)this)->Inherit(src);
    this->f28 = ((CCoinGameObstacleDef const *)src)->f28;
}