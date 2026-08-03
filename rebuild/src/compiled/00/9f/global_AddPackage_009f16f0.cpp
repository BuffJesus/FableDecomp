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

extern "C" void* memcpy(void*, const void*, unsigned int);

void CGameEventPackageSet::AddPackage(CGameEventPackage const* package)
{
    CGameEventPackageSetOverlay* const set = reinterpret_cast<CGameEventPackageSetOverlay*>(this);
    unsigned char* destination =
        reinterpret_cast<unsigned char*>(this) + set->NoPackages * 0x648 + 0x4;

    memcpy(destination, package, 0x192 * 4);

    set->NoPackages = set->NoPackages + 1;
}