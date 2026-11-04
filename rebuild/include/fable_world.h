#pragma once

// CWorld's polymorphic interface as retail dispatches through it.
//
// engine/CWorld.h is the generated PDB layout (a flat struct with no virtuals,
// used by the landed CWorld accessors); a TU that needs CWorld's *virtual*
// slots includes this header instead. The two must not meet in one TU.
//
// Evidence: retail RTTI (.?AVCWorld@@ type descriptor 0x01378AA4) gives two
// vtables, matching Ego_r.pdb's ??_7CWorld@@6BCBaseClassNonCopyable@@@ and
// ??_7CWorld@@6BCIDrawWorld@@@ slot for slot:
//   primary 0x012390F0 (CBaseClassNonCopyable subobject, +0x00):
//     0 ??_G 0x004AE8D0, 1 PostLoadInit 0x0049D970, 2 LoadWorldFromFile
//     0x0049E220, 3 GetThingManager 0x0049E1B0, 4 PeekThingManager 0x0049E1C0,
//     5 GetCurrentRenderCamera(const CInterpolationInfo&), 6 GetCurrentRenderCamera(),
//     7 PostInit, 8 GetGameLODCentre, 9 Init, 10 Uninitialise,
//     11 PreparePrimitivesForEngine  (slots 5-11 named from Ego_r's vtable)
//   secondary 0x012390DC (CIDrawWorld subobject, +0x04):
//     0 DrawGetProceduralTexture 0x004AE890 (xor eax,eax), 1 DrawGetEnvironment
//     0x0049E350, 2 DrawGetWorldMap 0x0049E1A0, 3 ??_G thunk 0x004AE8A0.
// Retail sizeof(CWorld) is 0x198 (operator new in CMainGameComponent::InitWorld
// 0x0041735A); Ego_r's is 0x1B8. Only the members retail code here reads are
// modelled; offsets agree with Ego_r up to +0xA4.

#include "fable_base_class.h"
#include "fable_class.h"
#include "fable_thing_components.h"
#include "fable_vector_map.h"
#include <bitset>
#include "fable_counted_pointer.h"
#include "fable_scoped_ptr.h"
#include "fable_string.h"
#include "fable_game_text_bank.h"
#include "rebuild_abi.h"

class C3DVector;
class CAIGameCameraBase;
class CMeshDataBank;
class CAFile;
class CBulletTimeManager;
class CCamera;
class CDisplayEngine;
class CEnginePrimitivePrepareInfo;
class CIDrawEnvironment;
class CIDrawWorldMap;
class CInterpolationInfo;
class CProceduralTexture;
class CThingManager;
class CWorldMap;
class CEnvironment;
class CWideString;
class CGameEvent;
class CGameEventPackage;
class CGameDefinitionManager;
class CMainGameComponent;
class CPlayer;
class CPlayerManager;
class CWorld;

// Ego_r.pdb CWorldInit (0x14). CMainGameComponent::InitWorld 0x0041735A
// builds it on the stack (ScavengeWorld, DefinitionManager and DisplayEngine
// zeroed first, then Component, PlayerManager and DefinitionManager filled).
class CWorldInit
{
public:
    CWorldInit()
        : ScavengeWorld(0),
          DefinitionManager(0),
          DisplayEngine(0)
    {
    }

    CMainGameComponent* Component;                  // +0x00
    CWorld* ScavengeWorld;                          // +0x04
    CPlayerManager* PlayerManager;                  // +0x08
    CGameDefinitionManager* DefinitionManager;      // +0x0C
    CDisplayEngine* DisplayEngine;                  // +0x10
};

// ?SetCurrentThingManager@CThing@@SIXAAVCThingManager@@@Z, retail 0x004C74F0.
// Partial retail CThing prefix for these methods; full layout is quarantined.
class CThing : public CClass
{
public:
    static void FABLE_FASTCALL SetCurrentThingManager(CThingManager& manager);
    static CThingManager* FABLE_FASTCALL GetPCurrentThingManager() { return PCurrentThingManager; }
    static const CThingManager* FABLE_FASTCALL PeekPCurrentThingManager() { return PCurrentThingManager; }
    const CGameDefinitionManager& PeekDefinitionManager() const; // retail 004C7990
    void Kill(bool force); // retail 004C9B80, FableWin CThing member
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    // Retail emits an out-of-line provider at 00436990 and inlines this
    // lookup in selected rendering callers. Keep established callers' ABI
    // unless their native body demonstrates that inline expansion.
#if defined(FABLE_THING_INLINE_CONST_GETTC) || defined(FABLE_THING_CONST_GETTC_PROVIDER)
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#else
    const CTCBase& GetTC(ETCInterfaceType type) const; // retail 00436990
#endif
    template<class T> __forceinline bool GetTC(T** component) {
        if (!HasTC(T::StatGetInterfaceType())) return false;
        *component = static_cast<T*>(&GetTC(T::StatGetInterfaceType()));
        return true;
    }


    // FableWin ?IsDead@CThing@@QBE_NXZ; inline in retail (CWorld::ActivateWorld
    // 0x0049F180 tests bit 0 of +0x91: Ego_r's Dead bitfield, +0xA1 there).
    bool IsDead() const
    {
        return Dead;
    }

private:
    // Primary CClass prefix is typed; remaining fields stay opaque.
    friend struct CThingComponentTestAccess;
    static CThingManager* PCurrentThingManager; // retail 013B8A1C
    fable_u8 unknown11_[0x18 - 0x11];
    unsigned __int64 UniqueID; // +18
    std::bitset<274> UsingComponents; // +20, 36-byte retail storage
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components; // +44
    fable_u8 unknown54_[0x91 - 0x54];
    unsigned char Dead : 1;                                      // +0x91 bit 0
};
FABLE_STATIC_ASSERT(sizeof(std::bitset<274>)==0x24);

// Retail RTTI chain CThingPlayerCreature : ... : CThing; only the CThing base
// is modelled.
class CThingPlayerCreature : public CThing
{
};

// FableWin ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z
// (a template over thing pointer types); inline in retail.
template<typename T>
inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->IsDead();
}

class CIDrawWorld
{
public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const;
    virtual const CIDrawEnvironment* DrawGetEnvironment() const = 0;
    virtual const CIDrawWorldMap* DrawGetWorldMap() const = 0;
    virtual ~CIDrawWorld();
};

class CWorld : public CBaseClassNonCopyable, public CIDrawWorld
{
public:
    // ??0CWorld@@QAE@ABVCWorldInit@@@Z, retail 0x004A67D0 (caller
    // CMainGameComponent::InitWorld 0x0041735A).
    explicit CWorld(const CWorldInit& init);
    virtual ~CWorld();

protected:
    virtual void PostLoadInit();

public:
    virtual void LoadWorldFromFile(CAFile& file, long version);
    virtual CThingManager& GetThingManager();
    virtual const CThingManager& PeekThingManager() const;
    // MSVC lays overloaded virtuals out in reverse declaration order:
    // slot 5 is the CInterpolationInfo overload, slot 6 the plain one.
    virtual CCamera GetCurrentRenderCamera() const;
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;
    virtual void PostInit(CDisplayEngine& displayEngine);
    virtual C3DVector GetGameLODCentre() const;
    virtual void Init();
    virtual void Uninitialise();
    virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);

    // Ego_r ?ChangeResolution@CWorld@@QAEXKKK@Z, retail 0x0049D820 (empty).
    void ChangeResolution(unsigned long width, unsigned long height, unsigned long depth);
    // Slots 12/13 (retail 0x004AE8B0 / 0x004AE8C0, both landed; FableWin
    // ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ / ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ).
    // CMainGameComponent::Run 0x004189C2 calls PeekWorldMap through [vptr+0x34].
    virtual CWorldMap& GetWorldMap();
    virtual const CWorldMap& PeekWorldMap() const;

    virtual const CProceduralTexture* DrawGetProceduralTexture() const;
    virtual const CIDrawEnvironment* DrawGetEnvironment() const;
    virtual const CIDrawWorldMap* DrawGetWorldMap() const;

    // ?GetFrame@CWorld@@SIJXZ: retail 0x0049D870 (`mov eax,[0x013B89BC]; ret`;
    // the manifest's GFGetPreMainMemoryUsed label is a byte-identical-body
    // propagation — Ego_r's CWorld::GetFrame 0x004A6BD0 is the same body).
    static long FABLE_FASTCALL GetFrame();

protected:
    // ?Frame@CWorld@@1JA (FableWin/Ego_r: protected static long CWorld::Frame),
    // retail 0x013B89BC. Static storage: no effect on the layout.
    static long Frame;

public:
    // Callees of CMainGameComponent's event and graphics paths (Ego_r.pdb
    // names by call site; retail addresses from the retail callers).
    bool EventIsWorldEvent(const CGameEvent* event) const;             // 0x0049D8C0 ?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z
    void ReceiveLocalEvent(const CGameEvent* event);                   // 0x0049E1D0 ?ReceiveLocalEvent@CWorld@@QAEXPBVCGameEvent@@@Z
    unsigned long GetChecksum() const;                                 // 0x0049E200 ?GetChecksum@CWorld@@QBEKXZ
    void Update(const CGameEventPackage& package, bool allowRender);   // 0x0049DFB0 ?Update@CWorld@@QAEXABVCGameEventPackage@@_N@Z
    void ChangeTextureColourDepth(long depth);                         // 0x0049D830 ?ChangeTextureColourDepth@CWorld@@QAEXJ@Z

    // ?RenderUpdate@CWorld@@QAEXPBVCInterpolationInfo@@@Z, retail 0x0049E080.
    void RenderUpdate(const CInterpolationInfo* info);

    // ?UpdateInputRelatedInterpolation@CWorld@@QAEXXZ, retail 0x0049D9E0
    // (an empty `ret` body in retail; CMainGameComponent::Update calls it).
    void UpdateInputRelatedInterpolation();

    // ?IsPaused@CWorld@@QBE_NXZ; inline in retail (`cmp byte [world+0xA4]`,
    // CMainGameComponent::Update 0x00418289).
    bool IsPaused() const
    {
        return Paused;
    }

    // Called by CMainGameComponent::PostInit 0x00416953 (FableWin names;
    // retail addresses by call order against FableWin's PostInit 0x018A4333).
    void Load(const CWideString& worldName);                              // 0x004A1840 ?Load@CWorld@@QAEXABVCWideString@@@Z
    bool LoadGameState(const CWideString& saveName, bool flag);           // 0x004A3200 ?LoadGameState@CWorld@@QAE_NABVCWideString@@_N@Z
    void ActivateWorld(bool flag);                                        // 0x0049F180 ?ActivateWorld@CWorld@@QAEX_N@Z
    CWideString ConvertWorldToStaticMapPath(const CWideString& worldName) const;   // 0x0049DDD0
    CWideString ConvertWorldToStaticMapPath(
        const CWideString& worldName, bool flag1, bool flag2) const;             // 0x0049D550
    CWideString ConvertWorldToISMThingyPath(const CWideString& worldName) const;   // 0x0049DEC0
    CWideString ConvertWorldToISMThingyPath(
        const CWideString& worldName, bool flag1, bool flag2) const;             // 0x0049D6B0

    // ?PeekGameCamera@CWorld@@QBEPBVCAIGameCameraBase@@XZ and
    // ?GetMeshBank@CWorld@@QAEPAVCMeshDataBank@@XZ; inline in retail
    // (CMainGameComponent::GetInputs 0x00416E78 reads world+0x34,
    // InitDisplayEngine 0x00417418 reads world+0x3C).
    const CAIGameCameraBase* PeekGameCamera() const
    {
        return PCurrentGameCamera;
    }

    CMeshDataBank* GetMeshBank()
    {
        return PMainMeshBank.Get();
    }

    // FableWin ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ and
    // ?SetAsStarting3DRendering@CWorld@@QAEXXZ; inline in retail
    // (ActivateWorld 0x0049F180 reads world+0x18 and stores Frame at +0x8C).
    CEnvironment& GetEnvironment()
    {
        return *PEnvironment;
    }

    void SetAsStarting3DRendering()
    {
        FrameStarted3DRendering = Frame;
    }

protected:
    // FableWin ?InitQuests@CWorld@@IAEXXZ; inline in retail (ActivateWorld
    // 0x0049F180), defined in that TU.
    void InitQuests();

private:
public:
    CMainGameComponent& Component; // +08, original FableWin reference
private:
public:
    CPlayerManager* PlayerManager;                              // +0x0C (Ego_r agrees)
private:
    fable_u8 unknown10_[0x14 - 0x10];
public:
    CWorldMap* PWorldMap;                                        // +0x14
private:
public:
    // Ego_r.pdb boost::scoped_ptr<CEnvironment> PEnvironment (+0x18);
    // CParticleEnvironmentInteraction::GetWindVelocity 0x0049BE10 reads
    // world+0x18 and calls its virtuals (see fable_environment.h).
    boost::scoped_ptr<CEnvironment> PEnvironment;               // +0x18
private:
    fable_u8 unknown1C_[0x34 - 0x1C];
public:
    CAIGameCameraBase* PCurrentGameCamera;                      // +0x34
private:
    fable_u8 unknown38_[0x3C - 0x38];
public:
    CCountedPointer<CMeshDataBank> PMainMeshBank;               // +0x3C
private:
    fable_u8 unknown44_[0x50 - 0x44];
public:
    CThingManager* PThingManager;                                // +0x50
private:
    fable_u8 unknown54_[0x68 - 0x54];
public:
    boost::scoped_ptr<CBulletTimeManager> PBulletTimeManager;   // +0x68
private:
    fable_u8 unknown6C_[0x80 - 0x6C];
public:
    bool JustLoaded;                                            // +0x80
private:
    fable_u8 unknown81_[0x8C - 0x81];
public:
    long FrameStarted3DRendering;                               // +0x8C (Ego_r agrees)
private:
    fable_u8 unknown90_[0xA4 - 0x90];
public:
    bool Paused;                                                // +0xA4
private:
public:
    bool SlowMotion; // retail +0xA5; FableWin name (donor +0xAD).
private:
    fable_u8 unknownA6_[0xAC - 0xA6];
public:
    CArray<CCharString> InitialActiveQuests;                    // +0xAC (Ego_r agrees)
private:
    fable_u8 unknownB8_[0x140 - 0xB8];
public:
    // Ego_r puts this at +0x150; retail's is +0x140: AutoSaveCheckPoint
    // 0x004A0BB0 sets it to 1 and returns it, AutoSaveQuestStart 0x004A0D53
    // and PrepareForLoad 0x004A0343 clear it, the ctor 0x004A67D0 zeroes it.
    bool AutoSaveCheckPointExists;                              // +0x140

    // FableWin ?SetAutoSaveCheckPointExists@CWorld@@QAEX_N@Z; inline in
    // retail (CMainGameComponent::Init 0x004184BD stores the byte directly).
    void SetAutoSaveCheckPointExists(bool exists)
    {
        AutoSaveCheckPointExists = exists;
    }

    // FableWin ?DeleteCheckPointSave@CWorld@@SIXXZ, retail 0x0049E740.
    static void FABLE_FASTCALL DeleteCheckPointSave();

public:
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event); // retail 0049EAF0
protected:
    CPlayer* GetPlayer(unsigned long number); // retail 0049D850
    void UpdateFrame(); // retail 004A5A40
    void SetAsPaused(bool paused); // retail 0049D8F0
public:
    // FableWin event-dispatch members; retail table initializer 0121B9B0.
    void EAUpdateFrame(const CGameEvent* event);
    void LEASetExclusiveMode(const CGameEvent* event);
    void LEASetDisplayMode(const CGameEvent* event);
    void LEASetEditorMode(const CGameEvent* event);
    void EAFirstPersonViewStart(const CGameEvent* event);
    void EACreatureCheat(const CGameEvent* event);
    bool AutoLoadRespawn(); // retail 004A5E90
    void ResetToFrontEnd(); // retail 0049DCC0
    void EAPlayerRespawn(const CGameEvent* event);
    void EASetPauseMode(const CGameEvent* event);
    void EASetSlowMotion(const CGameEvent* event);
    void EASetFreeCameraMode(const CGameEvent* event);
    void EAUseFreeCamera(const CGameEvent* event);
    void EACreatureBlock(const CGameEvent* event);
    void EATempWorldEventForWhateverYouWant(const CGameEvent* event);
    void EAApplyAllScriptedMapBrushes(const CGameEvent* event);
    void EAMoveCreature(const CGameEvent* event);
    void EACreatureUseObject(const CGameEvent* event);
    void EAControlledCreatureTalk(const CGameEvent* event);
    void EAControlledCreatureLearnExpression(const CGameEvent* event);
    void EAControlledCreatureDropObject(const CGameEvent* event);
    void EAControlledCreatureZTarget(const CGameEvent* event);
    void EAControlledCreatureUseAbility(const CGameEvent* event);
    void EAControlledCreatureStartSneaking(const CGameEvent* event);
    void EAControlledCreatureStopSneaking(const CGameEvent* event);
    void EAClickPastText(const CGameEvent* event);
    void EAOpenHeroInformationScreen(const CGameEvent* event);
    void EACloseHeroInformationScreen(const CGameEvent* event);
    void EACloseInGameMenu(const CGameEvent* event);
    void EAAnswerQuestion(const CGameEvent* event);
    void EAControlledCreatureStartThrowObject(const CGameEvent* event);
    void EAControlledCreatureEndThrowObject(const CGameEvent* event);
    void EAControlledCreatureSheatheWeapon(const CGameEvent* event);
    void EAControlledCreatureUnsheatheMeleeWeapon(const CGameEvent* event);
    void EAControlledCreatureUnsheatheRangedWeapon(const CGameEvent* event);
    void EAControlledCreaturePlayerInteraction(const CGameEvent* event);
    void EAControlledCreatureGiveItemToTarget(const CGameEvent* event);
    void EAControlledCreatureUnfreezeControls(const CGameEvent* event);
    void EAControlledCreatureRoll(const CGameEvent* event);
    void EAUseQuickAccessItem(const CGameEvent* event);
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event);
    void EAChargeQuickAccessItem(const CGameEvent* event);
    void EAPutAway(const CGameEvent* event);
    void EAControlledCreatureStrafe(const CGameEvent* event);
    void EAMoveHeroToRegion(const CGameEvent* event);
    void EAFirstPersonTargeting(const CGameEvent* event);
    void EAControlledCreatureDefault(const CGameEvent* event);
    void EAExpressionFollow(const CGameEvent* event);
    void EAExpressionWait(const CGameEvent* event);
    void EAUseProjectileWeapon(const CGameEvent* event);
    void EAControlledCreatureJump(const CGameEvent* event);
    void EAMoveSpirit(const CGameEvent* event);
    void EAOpenHeroCentreDoor(const CGameEvent* event);
    void EACloseHeroCentreDoor(const CGameEvent* event);
    void EASpiritAttack(const CGameEvent* event);
    void EACreatureSprint(const CGameEvent* event);
    void EASpiritReturnToHero(const CGameEvent* event);
    void EASkipCutScene(const CGameEvent* event);
    void EAUseProjectileWeaponThirdPerson(const CGameEvent* event);
    void EAChargeUpWillSpell(const CGameEvent* event);
    void EALoadGameFromInGameMenu(const CGameEvent* event);
    void EAControlledCreatureBlockEnd(const CGameEvent* event);
    void EAControlledCreatureDeactivateZTarget(const CGameEvent* event);
    void EARemoveCurrentMode(const CGameEvent* event);
    void EAControlledCreatureLightning(const CGameEvent* event);
    void EAControlledCreatureDeactivateLightning(const CGameEvent* event);
    void EACloseLiveGUI(const CGameEvent* event);
    void EAClosePhotoCapture(const CGameEvent* event);
    void EATakeTheBloodyScreenshot(const CGameEvent* event);
    void EAConsoleToggleState(const CGameEvent* event);
    void EAOpenPCSkillsMenu(const CGameEvent* event);
    void EAOpenPCMSNChatMenu(const CGameEvent* event);
    void EAOpenPCInventoryMenu(const CGameEvent* event);
    void EAOpenPCOptionsMenu(const CGameEvent* event);
    void EAOpenPCPhotoJournalMenu(const CGameEvent* event);
    void EAOpenPCMapMenu(const CGameEvent* event);
    void EAOpenPCBuyTradingMenu(const CGameEvent* event);
    void EAOpenPCSellTradingMenu(const CGameEvent* event);
    void EAOpenPCWantedTradingMenu(const CGameEvent* event);
    void EAOpenPCStatusMenu(const CGameEvent* event);
    void EAOpenPCScoreboard(const CGameEvent* event);
    void EADeleteMenuComponents(const CGameEvent* event);
    void EATogglePassiveAggressiveMode(const CGameEvent* event);
    void EADeleteTradeActiveMenu(const CGameEvent* event);
    void EAToggleKillEverythingMode(const CGameEvent* event);
    void EAToggleViewHeroMode(const CGameEvent* event);
    void EACentreCamera(const CGameEvent* event);
private:
    fable_u8 unknown141_[0x198 - 0x141];
};

FABLE_STATIC_ASSERT(sizeof(CWorld) == 0x198);

// Original FableWin global bool; retail 0137D76C.
extern bool GUnpauseAfterClosingHeroInformationScreen;
