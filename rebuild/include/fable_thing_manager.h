#pragma once
#include "fable_base_class.h"
class CGameDefinitionManager;
class CMainGameComponent;
class CWorld;
class CWorldMap;
class CPlayerManager;
// Original primary ancestry. Opaque fields cover the retail factory prefix;
// no complete allocation or class-type-info representation is claimed.
// CThingManager below models only its prefix through PlayerManager.
class CClass;
class CClassTypeInit;
class CClassFactory : public CBaseClassNonCopyable {
protected:
    // FableWin protected virtuals, retail vtable order (CNavigatorManager vtable
    // 0x0129CA84 slots 1..3): AddNewClassType 0x00A36F90, OnCreateClass
    // 0x0051DA20, OnDeleteClass 0x0051DA30.
    virtual void AddNewClassType(const CClassTypeInit& init);
    virtual void OnCreateClass(CClass& item);
    virtual void OnDeleteClass(CClass& item);
private:
    fable_u8 unknown04_[0x1C-4];
};
class CThingManager : public CClassFactory {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const { return DefinitionManager; }
    unsigned long GetNonLocalThingChecksum() const;
    CWorld& GetWorld() { return World; }
    const CWorld& PeekWorld() const { return World; }
    CPlayerManager& GetPlayerManager() { return PlayerManager; }
    const CPlayerManager& PeekPlayerManager() const { return PlayerManager; }
    const CMainGameComponent& PeekComponent() const { return Component; }
private:
    const CMainGameComponent& Component; // retail +1C; Ego_r +28
    const CGameDefinitionManager& DefinitionManager; // retail +20; Ego_r +2C
    CWorld& World; // retail +24; Ego_r +30
    CWorldMap& WorldMap; // retail +28; Ego_r +34
    unsigned long* WorldSeed; // retail +2C; Ego_r +38
    CPlayerManager& PlayerManager; // retail +30; Ego_r +3C
};

FABLE_STATIC_ASSERT(sizeof(CClassFactory)==0x1C);
FABLE_STATIC_ASSERT(sizeof(CThingManager)==0x34);
