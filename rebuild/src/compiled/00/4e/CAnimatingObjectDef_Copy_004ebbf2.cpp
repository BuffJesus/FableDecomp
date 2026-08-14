// CAnimatingObjectDef::Copy 0x004ebbf2 reconstruction
struct CDefClassBase;

struct SubMember {
    char pad[4];
    void Copy(const SubMember* src);
};

struct CDefClassBase {
    char pad[0x28];
    SubMember member;
};

struct CAnimatingObjectDef : public CDefClassBase {
    void CopyBase(const CDefClassBase* src);
    void Copy(const CDefClassBase* src);
};

void CAnimatingObjectDef::Copy(const CDefClassBase* src)
{
    CopyBase(src);
    this->member.Copy(&src->member);
}