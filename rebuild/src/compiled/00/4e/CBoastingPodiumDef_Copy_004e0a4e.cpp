struct CDefClassBase {
    int pad[10];
    unsigned int field28; // 0x28
    void Copy(const CDefClassBase* src); // __fastcall base copy (external)
};

struct CBoastingPodiumDef : CDefClassBase {
    void Copy(const CDefClassBase* src);
};

void CBoastingPodiumDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::Copy(src);
    this->field28 = src->field28;
}