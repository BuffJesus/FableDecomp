#pragma once

// CPlayerManager / CGamePlayerInterface as polymorphic classes (their virtual
// destructors are what boost::scoped_ptr<> deletes through in
// ~CMainGameComponent 0x00418B7A). engine/CPlayerManager.h and
// engine/CGamePlayerInterface.h are the generated flat PDB layouts (no
// virtuals); a TU includes either those or this header, never both.
//
// Retail RTTI:
//   CPlayerManager : CBaseClassNonCopyable; vtable 0x01231CD0 has one slot,
//     ??_G 0x0044C030 (dtor 0x0044A140).
//   CGamePlayerInterface : CInputProcessManager : CBaseClassNonCopyable;
//     vtable 0x01231BDC: ??_G 0x00449420 (dtor 0x00446BE0), then
//     GetGameEvent 0x00446A30 (FableWin ?GetGameEvent@CGamePlayerInterface@@UBE_NAAVCGameEvent@@@Z).
// Layouts and sizes are not modelled.

#include "fable_base_class.h"
#include "fable_init_base_class.h"
#include "fable_intelligent_pointer.h"
#include "fable_stlport_vector.h"
#include "fable_stlport_list.h"
#include "fable_string.h"
#include "rebuild_abi.h"

class CGameEvent;
class CGamePlayerInterface;

class CGameDefinitionManager;
class CMainGameComponent;

class CThingPlayerCreature;
class CPlayerManager;

// FableWin global bool; retail 013B85F3.
extern bool GUsePassiveAggressiveMode;

// FableWin EPlayerMode values used by recovered event handlers.
// Ego_r.pdb enums used only as parameter types here; their enumerators are not modelled (the EGameAction name table
// is stripped from retail).
enum EGameAction {};
enum EInputButtonEventType {};

enum EPlayerMode
{
    PLAYER_MODE_CONTROL_CREATURE = 1,
    PLAYER_MODE_Z_TARGET = 2,
    PLAYER_MODE_FIRST_PERSON = 16,
    PLAYER_MODE_FREEZE_CONTROLS = 17,
    PLAYER_MODE_CONTROL_CAMERA = 19,
    PLAYER_MODE_FIRST_PERSON_TARGETING = 22,
    PLAYER_MODE_NAVIGATE_IN_GAME_MENU = 23,
    PLAYER_MODE_LIGHTNING = 40,
    PLAYER_MODE_LIVE_GUI = 43,
    PLAYER_MODE_CONSOLE = 44,
    PLAYER_MODE_TAKE_SCREENSHOT_FOR_PHOTOJOURNAL = 45,
    PLAYER_MODE_VIEW_HERO = 47
};

// FableWin CPlayer : CInitBaseClass. Only the retail prefix through
// UsingFreeCamera (+220) is modelled; no full-class allocation or size claim.
class CPlayer : public CInitBaseClass
{
public:
    // FableWin ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
    // (retail 0x00487DC0).
    CThingPlayerCreature* GetPControlledCreature() const;
    const CThingPlayerCreature* PeekPControlledCreature() const; // retail 00487DD0
    bool IsMainPlayer() const; // retail 00487520
    bool HasMode(EPlayerMode mode) const; // retail 00634160
    EPlayerMode GetCurrentMode() const; // retail 00633BE0
    void AddMode(EPlayerMode mode, bool force); // retail 006345C0
    void RemoveMode(EPlayerMode mode); // retail 00634190
    void UnfreezeControls(); // retail 00487120
    void ToggleAggressiveMode(); // retail 00487E90
    long GetNumber() const { return Number; } // FableWin inline member
    bool IsAggressiveMode() const { return AggressiveMode; } // FableWin inline member
    bool IsKillEverythingMode() const { return KillEverythingMode; }
    void SetKillEverythingMode(bool enabled) { KillEverythingMode = enabled; }
    void SetUsingFreeCamera(bool enabled) { UsingFreeCamera = enabled; }
    // Ego_r ?GetJoystickDeviceNumber@CPlayer@@QBEJXZ (retail 0x00487510).
    long GetJoystickDeviceNumber() const;
private:
    friend struct CPlayerLookupTestAccess;
    friend struct CPlayerControlTestAccess;
    fable_u8 unknown08_[0x0C - 0x08];
    CGamePlayerInterface* PlayerInterface; // retail/FableWin +0C
    CPlayerManager* PlayerManager; // retail/FableWin +10
    fable_u8 unknown14_[0x28 - 0x14];
    long Number; // retail/FableWin +28
    CIntelligentPointer<CThingPlayerCreature> ControlledCreature; // retail/FableWin +2C
    fable_u8 unknown34_[0x210 - 0x34];
    _STL::list<EPlayerMode> PlayerModes; // retail +210, 4-byte STLport list
    bool SuppressFullModeRemoval; // retail +214
    bool DisallowModeChanges; // retail +215
    bool InitialPlayerModeSet; // retail +216
    bool AggressiveMode; // retail +217; FableWin +223
    fable_u8 unknown218_[0x21B - 0x218];
    bool KillEverythingMode; // retail +21B; FableWin +227
    fable_u8 unknown21C_[0x220 - 0x21C];
    bool UsingFreeCamera; // retail +220; FableWin +22C
    fable_u8 unknown221_[0x228 - 0x221];
    long JoystickDeviceNumber; // retail +228; Ego_r +230
    void SetUpControlsBasedOnAggressiveMode(); // retail 00487DE0
};

class CPlayerManager : public CBaseClassNonCopyable
{
public:
    // ??0CPlayerManager@@QAE@AAVCMainGameComponent@@ABVCGameDefinitionManager@@@Z,
    // retail 0x0044A3B0 (the manifest's CPlayerGui::SetMoralityAsDisplaying
    // label is a propagation; CMainGameComponent::InitPlayerManager 0x0041732A
    // news 0x2C bytes for it).
    CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& definitionManager);
    virtual ~CPlayerManager();

    // ?CreatePlayers@CPlayerManager@@QAEXXZ, retail 0x0044A530 (caller
    // CMainGameComponent::CreatePlayers 0x004166A8).
    void CreatePlayers();

    // FableWin ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (retail
    // 0x00449970), ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ (0x00449700) and
    // ?InitPlayerCharacter@CPlayerManager@@QAEX_N@Z (0x00449D90); caller
    // CWorld::ActivateWorld 0x0049F180.
    CPlayer& GetMainPlayer();
    CPlayer& GetPlayer(long number); // retail 004498C0 (FableWin returns reference)
    const CPlayer& GetPlayer(long number) const; // retail 00449910
    const CPlayer& GetMainPlayer() const; // retail 00449960
    long GetMainPlayerNumber() const;
    void InitPlayerCharacter(bool flag);

private:
    friend struct CPlayerLookupTestAccess;
    CMainGameComponent* Component; // +04
    const CGameDefinitionManager* DefinitionManager; // +08
    _STL::vector<CPlayer*> Players; // +0C, 12-byte retail STLport
    long PlayerNeutral; // +18: fallback player number/index
    long MainPlayer; // +1C
    _STL::vector<CCharString> HeroSwapPlayerScriptNames; // +20
};
FABLE_STATIC_ASSERT(sizeof(CPlayerManager) == 0x2C);

class CInputProcessManager : public CBaseClassNonCopyable
{
public:
    virtual ~CInputProcessManager();
};

class CMainGameComponent;

class CGamePlayerInterface : public CInputProcessManager
{
public:
    // FableWin ??0CGamePlayerInterface@@QAE@ABVCMainGameComponent@@ABVCPlayerManager@@@Z,
    // retail 0x004473A0 (CMainGameComponent::Init 0x004184BD news it).
    CGamePlayerInterface(const CMainGameComponent& component, const CPlayerManager& playerManager);
    virtual ~CGamePlayerInterface();
    virtual bool GetGameEvent(CGameEvent& event) const;

    // ?Update@CGamePlayerInterface@@QAEXXZ (retail 0x004457F0; caller
    // CMainGameComponent::GetInputs 0x00416E78).
    void Update();

    // FableWin inline member; retail UnfreezeControls writes +79C.
    void ResumeAllInputProcesses() { Suspended = false; }

    // Ego_r ?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@JAAM@Z
    // (retail 0x00445CB0, not yet reconstructed) and the overload without the time output (retail 0x00446B10).
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long joystick, float& time) const;
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long joystick) const;

private:
    // Retail sizeof is 0x898 (operator new(0x898) in CMainGameComponent::Init
    // 0x004184BD); Ego_r's is 0x8A8. Members not modelled.
    friend struct CPlayerControlTestAccess;
    fable_u8 unknown04_[0x79C - 4];
    bool Suspended; // retail +79C; FableWin +7A0, Ego_r name
    fable_u8 unknown79D_[0x898 - 0x79D];
};
FABLE_STATIC_ASSERT(sizeof(CGamePlayerInterface) == 0x898);
