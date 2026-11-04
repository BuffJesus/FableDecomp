#pragma once
#include "fable_base_class.h"
#include "fable_def_pointer.h"
class CCombatTypeDef;
#include "fable_counted_pointer.h"
#include "fable_stlport_list.h"
class CThing;
class CWorld;
class CPlayerManager;
class CMainGameComponent;
class CGameDefinitionManager;
class CThingCreatureBase;
enum ETCInterfaceType { TCI_HERO_STATS=4, TCI_REGION_DISPLAY=142, TCI_INVENTORY=17, TCI_INVENTORY_TRADE=23, TCI_COMBAT=48, TCI_ENTITY_MODE_MANAGER=49, TCI_STEALTH=54, TCI_LOOK=67, TCI_HERO_RANGED_ATTACKS=129, TCI_PHOTOJOURNAL_CAPTURE=270 };
class CTCBase : public CBaseClassNonCopyable {
public:
    static void FABLE_FASTCALL RegisterTCs(); // retail 004EE23F
    virtual ~CTCBase();
    CThing& GetParentThing() { return *ParentThing; }
    const CThing& GetParentThing() const { return *ParentThing; }
private:
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
protected:
    const CGameDefinitionManager& GetDefinitionManager() const; // retail 006869D0
    CWorld& GetWorld() const; // retail 00686A70
    const CWorld& PeekWorld() const; // retail 00686A80
    CPlayerManager& GetPlayerManager(); // retail 00686D40
    const CPlayerManager& PeekPlayerManager() const; // retail 00686D50
    const CMainGameComponent& PeekComponent() const; // retail 00686D60
    unsigned long* GetPLocalSeed() const; // retail 00686DB0
};
namespace NInventory {
// Ego_r/FableWin enum; the trade categories are passed at retail 0059D9AD.
enum EInventoryCategoryIdentifier {
    INVENTORY_CATEGORY_NONE=0,
    INVENTORY_CATEGORY_IDENTIFIER_EXPERIENCE_STRENGTH=1,
    INVENTORY_CATEGORY_IDENTIFIER_EXPERIENCE_SKILL=2,
    INVENTORY_CATEGORY_IDENTIFIER_EXPERIENCE_WILL_WEAPON_MAGIC=3,
    INVENTORY_CATEGORY_IDENTIFIER_EXPERIENCE_WILL_ABILITY_MAGIC=4,
    INVENTORY_CATEGORY_IDENTIFIER_EXPERIENCE_WILL_PURE_MAGIC=5,
    INVENTORY_CATEGORY_IDENTIFIER_MAP_WORLD_MAP=6,
    INVENTORY_CATEGORY_IDENTIFIER_MAP_REGIONS=7,
    INVENTORY_CATEGORY_IDENTIFIER_MAP_QUESTS=8,
    INVENTORY_CATEGORY_IDENTIFIER_MAP_TELEPORTS=9,
    INVENTORY_CATEGORY_IDENTIFIER_MAP_HOUSES=10,
    INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY=11,
    INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL=12,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_FLAME=13,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_EXTRA_DAMAGE=14,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_SILVER=15,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_LIGHTNING=16,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_DIAMOND=17,
    INVENTORY_CATEGORY_IDENTIFIER_AUGMENT_ADD_SLOT=18,
    INVENTORY_CATEGORY_MAX_IDENTIFIERS=19
};
// Original primary chain; only CTCBase prefix modeled here.
class CTCInventoryBase : public CTCBase {
public:
};
}
class CTCInventory : public NInventory::CTCInventoryBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_INVENTORY; }
    // Secondary NHeroInformationScreens::CBase is outside this partial prefix.
    void OpenPCScoreboard(); // retail 00587B93
    void OpenPCSkillsMenu(); // retail 00587BFF
    void OpenPCInventoryMenu(); // retail 0058807D
    void OpenPCOptionsMenu(); // retail 0058AF10
    void OpenPCPhotojournal(); // retail 0058F9BC
    void OpenPCMapMenu(); // retail 00589A30
    void OpenPCStatusMenu(); // retail 0058A763
    void DeleteMenuComponents(); // retail 00587A41
};

// Original primary ancestry; secondary hero-screen base and complete layout omitted.
class CTCInventoryTrade : public NInventory::CTCInventoryBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_INVENTORY_TRADE; }
    void OpenActualBuyInventoryPC(); // retail 005AE972
    void OpenActualSellInventoryPC(); // retail 005AC0EE
    void OpenActualWantedInventoryPC(); // retail 005AC0FE
    void DeleteComponents();
    void ShowWarningIfNeeded(); // retail 0059DABB
    void ResetInventoryTradeItems(); // retail 0059D9AD
    void RemoveAllItemsFromCategory(NInventory::EInventoryCategoryIdentifier category); // retail 0059CF49
};

// Original CTCBase primary prefix; full capture state is omitted.
class CTCPhotojournalCapture : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_PHOTOJOURNAL_CAPTURE; }
    void DestroyScreen(); // retail 0055D6C0
};

// Original primary CTCBase prefix; full combat state is omitted.
class CTCCombat : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_COMBAT; }
    CDefPointer<const CCombatTypeDef> GetPCombatTypeDef() const { return PCombatTypeDef; }
    bool IsAbleToBeKnockedDown() const; // retail 007331A0
    bool IsBlocking() const; // retail 007331E0
    bool IsVulnerableToUnblockableAttack() const; // retail 007331C0
    void UnsetBlocking(); // retail 007338A0
private:
    const CThingCreatureBase& GetParentCreature() const { return *ParentCreature; }
    fable_u8 unknown0C_[0x24-0x0C];
    CThingCreatureBase* ParentCreature; // retail/FableWin +24
    CDefPointer<const CCombatTypeDef> PCombatTypeDef; // retail/FableWin +28
    fable_u8 unknown2C_[0x62-0x2C];
    bool StartingToBlock; // retail +62; FableWin +7A
    bool Blocking; // retail +63; FableWin +7B
    long BlockHoldTimer; // retail +64; FableWin +7C
    long BlockingNumFrames; // retail +68; FableWin +80
};

// Original primary CTCBase prefix; complete ranged-attack state omitted.
class CTCHeroRangedAttacks : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_RANGED_ATTACKS; }
    void ResetProjectileWeaponMode(); // retail 00789B30
};


class CAnimateThing;
class CAnimFilterBase;
class C3DVector;
enum EMovementType { AI_MOVEMENT_NULL=0, AI_MOVEMENT_SNEAKING=1, AI_MOVEMENT_SLOW_WALKING=2, AI_MOVEMENT_WALKING=3, AI_MOVEMENT_JOGGING=4, AI_MOVEMENT_RUNNING=5, AI_MOVEMENT_NON_CONSTANT_RUNNING=6, AI_MOVEMENT_FLYING=7, AI_MOVEMENT_HOVER_SLOW=8, AI_MOVEMENT_HOVER_MEDIUM=9, AI_MOVEMENT_HOVER_FAST=10 };
// Original primary prefix and retail list/time fields; available idle modes omitted.
class CTCCreatureModeManager : public CTCBase {
public:
    // Original twelve-slot virtual ABI. Complete mode state is omitted.
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const = 0;
        virtual void AnimUpdate(CAnimateThing& thing);
        virtual void FrameUpdate();
        virtual const CAnimFilterBase& GetAnimFilter() const;
        virtual void OnActivate();
        virtual void OnDeactivate();
        virtual float GetMovementSpeed(const C3DVector&,const C3DVector&,EMovementType) const;
        virtual bool IsIdleMode() const;
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;
        virtual bool IsHoldWeaponWithBothHands() const;
        virtual bool IsRemoveOnDeactivate() const;
        class CIsType {
        public:
            explicit CIsType(long type) : Type(type) {}
            bool operator()(const CCountedPointer<CModeBase>& mode) const { return mode->GetType() == Type; }
        private:
            long Type;
        };
    };
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    void RemoveMode(unsigned long mode); // retail 0073ABA0
private:
    friend struct CreatureModeManagerTestAccess;
    _STL::list<CCountedPointer<CModeBase> > PModes; // retail +0C; FableWin +0C
    long TimeLeftForCurrentMode; // retail +10; FableWin +1C
    long TimeForNextIdleMode; // retail +14; FableWin +20
};
// Primary prefix only; secondary graphic-appearance base is outside this model.
class CTCLook : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_LOOK; }
    void UpdateTurningAnims(); // retail 00754050
};

// Original CTCBase primary prefixes; complete component layouts omitted.
class CInterpolationInfo;
class CIEngine;
class CTCRegionDisplay : public CTCBase {
public:
    void GlobalDraw(const CInterpolationInfo& info) const; // retail 00639E40
};
class CTCHeroStats : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_STATS; }
    void RenderHeroDrunkenness(CIEngine* engine); // retail 0057B43F
};
