// CEngineLocalDetailGeneratorDef::Copy @ 0x00477288
struct CDefClassBase {
    char base_storage[0x3c];               // base subobject occupies +0x00..+0x3c
    void Copy(CDefClassBase const* src);   // 0x431ed0 (thiscall)
};

struct MemberSub {
    char pad[8];
    void Copy(MemberSub const* src);       // 0x4772a7 (thiscall, ret 4)
};

struct CEngineLocalDetailGeneratorDef : public CDefClassBase {
    MemberSub sub;   // at +0x3c
    void Copy(CDefClassBase const* src);
};

void CEngineLocalDetailGeneratorDef::Copy(CDefClassBase const* src)
{
    CDefClassBase::Copy(src);
    this->sub.Copy((MemberSub const*)((char const*)src + 0x3c));
}