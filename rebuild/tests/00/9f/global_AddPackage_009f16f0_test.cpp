#include <string.h>
#include <stdio.h>

struct CGameEventPackageOverlay {
    unsigned long Unknown0x00;
    unsigned long EventCount;
    unsigned char RawEvents[0x640];
};

struct CGameEventPackageSetOverlay {
    unsigned long NoPackages;
    CGameEventPackageOverlay Packages[50];
};

struct CGameEventPackage;

struct CGameEventPackageSet {
    void AddPackage(CGameEventPackage const* package);
};

void CGameEventPackageSet::AddPackage(CGameEventPackage const* package)
{
    CGameEventPackageSetOverlay* const set = reinterpret_cast<CGameEventPackageSetOverlay*>(this);
    unsigned long* destination = reinterpret_cast<unsigned long*>(
        reinterpret_cast<unsigned char*>(this) + set->NoPackages * 0x648 + 0x4);
    unsigned long const* source = reinterpret_cast<unsigned long const*>(package);

    for (int i = 0x192; i != 0; --i) {
        *destination = *source;
        ++source;
        ++destination;
    }

    set->NoPackages = set->NoPackages + 1;
}

static unsigned char raw[0x4 + 3 * 0x648 + 0x10];
static unsigned char srcbuf[0x648];

int main()
{
    memset(raw, 0xCD, sizeof(raw));

    CGameEventPackageSetOverlay* setOverlay = reinterpret_cast<CGameEventPackageSetOverlay*>(raw);
    setOverlay->NoPackages = 1;

    unsigned int i;
    for (i = 0; i < sizeof(srcbuf); ++i) {
        srcbuf[i] = (unsigned char)(i * 3 + 7);
    }

    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(raw);
    CGameEventPackage const* pkg = reinterpret_cast<CGameEventPackage const*>(srcbuf);

    set->AddPackage(pkg);

    int ok = 1;
    if (setOverlay->NoPackages != 2) ok = 0;

    unsigned char* dest = raw + 0x648 + 0x4;
    if (memcmp(dest, srcbuf, sizeof(srcbuf)) != 0) ok = 0;

    if (ok) {
        printf("BEHAVIOR_TEST_PASS_009F16F0\n");
    } else {
        printf("BEHAVIOR_TEST_FAIL_009F16F0\n");
    }
    return 0;
}