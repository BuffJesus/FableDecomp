#include "engine/CGameScriptInterface.h"

class CCharString;
class CIDrawEnvironment;
struct CWorldMapLookup { CIDrawEnvironment* GetRegionNumberFromName(const CCharString& regionName); };
struct CWorldEnvironment { CIDrawEnvironment* DrawGetEnvironment(); };
struct CWorldProvider { void** vtable; };
typedef void* (__fastcall* GetWorldObjectFn)(CWorldProvider* provider);
struct CGameScriptInterface_Methods : CGameScriptInterface {
    virtual bool IsRegionLoaded(const CCharString& regionName) const;
};

bool CGameScriptInterface_Methods::IsRegionLoaded(const CCharString& regionName) const {
    const CGameScriptInterface* scriptInterface =
        reinterpret_cast<const CGameScriptInterface*>(this);
    CWorldProvider* provider =
        reinterpret_cast<CWorldProvider*>(scriptInterface->World);
    CIDrawEnvironment* region =
        reinterpret_cast<CWorldMapLookup*>(
            ((GetWorldObjectFn)provider->vtable[0x34 / 4])(provider))
            ->GetRegionNumberFromName(regionName);

    CWorldProvider* provider2 =
        reinterpret_cast<CWorldProvider*>(scriptInterface->World);
    CIDrawEnvironment* current =
        reinterpret_cast<CWorldEnvironment*>(
            ((GetWorldObjectFn)provider2->vtable[0x34 / 4])(provider2))
            ->DrawGetEnvironment();
    return (bool)(region == current);
}
