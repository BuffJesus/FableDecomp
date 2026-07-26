
#include <cstdio>
#include <cstring>

struct CCharString {
    char* p;
    CCharString(const char* s) { p = (char*)s; }
};

struct CInner {
    virtual CCharString GetName() { return CCharString("VIRT"); }
};

struct CScriptThing {
    unsigned char _pad_0x0[0x4];
    CInner* inner;
    CCharString GetHomeMapName();
};

const char kHomeMapName[] = "HOME_MAP";

CCharString CScriptThing::GetHomeMapName()
{
    CInner* inner = this->inner;
    if (inner == 0)
        return CCharString(kHomeMapName);
    return inner->GetName();
}

int main()
{
    CScriptThing a;
    a.inner = 0;
    CCharString r1 = a.GetHomeMapName();
    if (std::strcmp(r1.p, "HOME_MAP") != 0) { std::printf("FAIL null-path\n"); return 1; }

    CInner ci;
    CScriptThing b;
    b.inner = &ci;
    CCharString r2 = b.GetHomeMapName();
    if (std::strcmp(r2.p, "VIRT") != 0) { std::printf("FAIL virt-path\n"); return 1; }

    std::printf("CScriptThing_004aaa10_TEST PASS\n");
    return 0;
}