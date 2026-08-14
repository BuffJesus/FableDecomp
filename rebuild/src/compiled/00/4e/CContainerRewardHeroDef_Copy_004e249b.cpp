// CContainerRewardHeroDef::Copy  0x004e249b

struct CDefClassBase { };

// member object at +0x28 with a Copy(const T&) method -> 0x4e24ba
struct Member28 {
    int pad[4];
    void Copy(const Member28& src);
};

struct CContainerRewardHeroDef {
    char pad[0x28];
    Member28 m28;   // at +0x28
    void BaseCopy(const CDefClassBase& src); // -> 0x431f10 (base class Copy, this at +0)
    void Copy(const CDefClassBase& src);
};

void CContainerRewardHeroDef::Copy(const CDefClassBase& src)
{
    this->BaseCopy(src);
    const Member28* srcm = (const Member28*)((const char*)&src + 0x28);
    this->m28.Copy(*srcm);
}