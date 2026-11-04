#pragma once
#include "fable_def_class_base.h"
#include "fable_world.h"
// Original CParentDefClassBase primary chain; complete definition state omitted.
class CCombatTypeDef : public CParentDefClassBase {
public:
    fable_u8 unknown39_[0x9C-0x39];
    bool CanBeKnockedDown; // retail +9C; FableWin +C4
};
FABLE_STATIC_ASSERT(offsetof(CCombatTypeDef,CanBeKnockedDown)==0x9C);
// Original primary chains; intermediate state and complete allocations are omitted.
class CThingPhysical : public CThing {};
class CThingGameObject : public CThingPhysical {};
class CThingBaseDef : public CParentDefClassBase {
private:
    fable_u8 unknown39_[0x70-sizeof(CParentDefClassBase)];
};
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);
class CThingCreatureDef : public CThingBaseDef {
public:
    fable_u8 unknown70_[0x124-sizeof(CThingBaseDef)];
    long CombatTypeDef; // retail +124; FableWin +134
};
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDef,CombatTypeDef)==0x124);
// Only the proven prefix through PDef is modeled, not complete creature storage.
class CThingCreatureBase : public CThingGameObject {
public:
    CDefPointer<const CCombatTypeDef> GetPCombatTypeDef() const; // retail 00664D40
    const CCombatTypeDef& GetCombatTypeDef() const; // retail 00666D10
    long GetCombatType() const { return PDef->CombatTypeDef; }
private:
    fable_u8 unknown98_[0xE0-sizeof(CThingGameObject)];
    CDefPointer<const CThingCreatureDef> PDef; // retail +E0; FableWin +130
};
