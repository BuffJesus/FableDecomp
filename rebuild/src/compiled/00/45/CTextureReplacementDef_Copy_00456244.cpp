struct CSub { void Copy(const CSub* o); };
struct CTextureReplacementDef {
    char base[0x28];
    CSub sub;
    void BaseCopy(const CTextureReplacementDef* o);
    void CTextureReplacementDef_Copy(const CTextureReplacementDef* o);
};

void CTextureReplacementDef::CTextureReplacementDef_Copy(const CTextureReplacementDef* o)
{
    this->BaseCopy(o);
    this->sub.Copy(&o->sub);
}