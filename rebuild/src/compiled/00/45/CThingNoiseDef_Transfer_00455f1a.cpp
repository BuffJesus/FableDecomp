#pragma optimize("s",on)
struct CArchive {
    void Transfer(const void* name, void* field);
};
struct CThingDef {
    void Transfer(CArchive* ar);
};
struct CThingNoiseDef : public CThingDef {
    char extra[0x70];
    int noise;
    void Transfer(CArchive* ar);
};

extern const char kName[];

void CThingNoiseDef::Transfer(CArchive* ar)
{
    CThingDef::Transfer(ar);
    ar->Transfer(kName, &this->noise);
}