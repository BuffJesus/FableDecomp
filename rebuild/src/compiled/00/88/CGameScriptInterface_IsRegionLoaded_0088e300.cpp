class CCharString;
class CIDrawEnvironment;

struct CWorldMap {
    CIDrawEnvironment* GetRegionNumberFromName(const CCharString& regionName);
};

struct CWorld {
    CIDrawEnvironment* DrawGetEnvironment();
};

struct WorldProvider {
    void** vtable;
};

struct CGameScriptInterface {
    WorldProvider* worldProvider;
    virtual bool IsRegionLoaded(const CCharString& regionName) const;
};

typedef void* (__fastcall* GetWorldObjectFn)(WorldProvider* self);

bool CGameScriptInterface::IsRegionLoaded(const CCharString& regionName) const
{
    WorldProvider* wp = this->worldProvider;
    CIDrawEnvironment* regionEnv =
        ((CWorldMap*)((GetWorldObjectFn)wp->vtable[0x34 / 4])(wp))->GetRegionNumberFromName(regionName);

    WorldProvider* wp2 = this->worldProvider;
    CIDrawEnvironment* currentEnv =
        ((CWorld*)((GetWorldObjectFn)wp2->vtable[0x34 / 4])(wp2))->DrawGetEnvironment();

    return (bool)(regionEnv == currentEnv);
}