# CGSI Lua-binding candidates — DB-generated

*Generated 2026-07-21 from the RE DB (DumpVtableSlots.java -> cgsi_slots_aligned.tsv, joined with engine_api.tsv).*

ForgeFSE resolves `pInterface->pVTable[N]` where N is the vtable slot. This maps every useful
CGSI verb to its **slot N** + signature so a Lua binding can be added. The slot map was validated
against 7 live ForgeFSE anchors (Destructor=0, StartScriptingEntity=8, IsRegionLoaded=12, ...).
Full 921-slot map: `generated/fable_cgsi_slots.h` (constexpr slot per method).

**Note:** verbs showing `unknown undefined()` are named but their signature is not yet recovered in
the DB (37 of them). The **type/cc recovery pass (task #10)** would fill these; until then use the
verified signatures in SCRIPT_BINDINGS_CANDIDATES.md. 28 already have recovered signatures.

```
# CGSI bind candidates by category (slot | signature) — for adding ForgeFSE Lua bindings

## Conversation/dialogue (6)
  slot 364   CGameScriptInterface::AddNewConversation        __thiscall long(CGameScriptInterface *,CScriptThing *,bool,bool)
  slot 365   CGameScriptInterface::AddPersonToConversation   __thiscall void(CGameScriptInterface *,long,CScriptThing *)
  slot 366   CGameScriptInterface::AddLineToConversation     unknown undefined()
  slot 367   CGameScriptInterface::RemoveConversation        unknown undefined()
  slot 368   CGameScriptInterface::IsConversationActive      unknown undefined()
  slot 415   CGameScriptInterface::CameraDoConversation      __thiscall void(CGameScriptInterface *,CScriptThing *,CScriptThing *,ECamera)

## Property/house/door (11)
  slot 425   CGameScriptInterface::OpenDoor                  __thiscall void(CGameScriptInterface *,CScriptThing *)
  slot 426   CGameScriptInterface::CloseDoor                 __thiscall void(CGameScriptInterface *,CScriptThing *)
  slot 427   CGameScriptInterface::OpenHouseDoors            __thiscall void(CGameScriptInterface *,CScriptThing *)
  slot 428   CGameScriptInterface::CloseHouseDoors           __thiscall void(CGameScriptInterface *,CScriptThing *)
  slot 429   CGameScriptInterface::JamDoor                   __thiscall void(CGameScriptInterface *,CScriptThing *,long)
  slot 430   CGameScriptInterface::SetDoorTriggerType        __thiscall void(CGameScriptInterface *,CScriptThing *,long)
  slot 431   CGameScriptInterface::OverrideAutomaticHouseLocking  __thiscall void(CGameScriptInterface *,CScriptThing *,bool)
  slot 432   CGameScriptInterface::SetHouseOwnedByPlayer     unknown undefined()
  slot 433   CGameScriptInterface::SetBuyableHouseAsScripted  unknown undefined()
  slot 684   CGameScriptInterface::IsThingADoor              unknown undefined()
  slot 738   CGameScriptInterface::GetNumHousesOwned         unknown undefined()

## Minigame/skill (21)
  slot 55    CGameScriptInterface::MsgOnFishingGameFinished  __thiscall bool(CGameScriptInterface *,CScriptThing *)
  slot 243   CGameScriptInterface::IsFishingSpotEnabled      __thiscall bool(CGameScriptInterface *,CScriptThing *)
  slot 244   CGameScriptInterface::DisableFishingSpot        __thiscall void(CGameScriptInterface *,CScriptThing *)
  slot 247   CGameScriptInterface::HeroGoFishing             unknown undefined()
  slot 248   CGameScriptInterface::GetHeroFishingLevel       unknown undefined()
  slot 249   CGameScriptInterface::HeroGoDigging             unknown undefined()
  slot 250   CGameScriptInterface::HeroStopDigging           unknown undefined()
  slot 251   CGameScriptInterface::HeroPlayOracleMinigame    unknown undefined()
  slot 252   CGameScriptInterface::IsHeroPlayingOracleMinigame  unknown undefined()
  slot 253   CGameScriptInterface::HasHeroWonOracleMinigame  unknown undefined()
  slot 254   CGameScriptInterface::HeroPlayFireheartMinigame  unknown undefined()
  slot 255   CGameScriptInterface::HeroQuitFireheartMinigame  unknown undefined()
  slot 256   CGameScriptInterface::HasHeroForceQuitFireheartMinigame  unknown undefined()
  slot 342   CGameScriptInterface::DisplayMiniGameInfo       unknown undefined()
  slot 343   CGameScriptInterface::UpdateMiniGameInfoBar     unknown undefined()
  slot 575   CGameScriptInterface::GetNearestEnabledDiggingSpot  __thiscall CScriptThing(CGameScriptInterface *,CScriptThing *)
  slot 576   CGameScriptInterface::IsDiggingSpotEnabled      __thiscall bool(CGameScriptInterface *,CScriptThing *)
  slot 577   CGameScriptInterface::IsDiggingSpotHidden       __thiscall bool(CGameScriptInterface *,CScriptThing *)
  slot 578   CGameScriptInterface::SetDiggingSpotAsHidden    __thiscall void(CGameScriptInterface *,CScriptThing *,bool)
  slot 728   CGameScriptInterface::UpdateOnlineScore_FishingCompo  unknown undefined()
  slot 729   CGameScriptInterface::UpdateScore_FishingCompetition  unknown undefined()

## Entity/UID query (8)
  slot 72    CGameScriptInterface::GetThingWithScriptName    __thiscall CScriptThing(CGameScriptInterface *,CCharString *)
  slot 73    CGameScriptInterface::GetThingWithScriptName    __thiscall CScriptThing(CGameScriptInterface *,CCharString *,CCharString *)
  slot 83    CGameScriptInterface::GetThingWithUID           unknown undefined()
  slot 85    CGameScriptInterface::GetAllThingsInLevel       __thiscall long(CGameScriptInterface *,CCharString *,vector<CScriptThing,std)
  slot 86    CGameScriptInterface::IsThingWithThisUIDAlive   unknown undefined()
  slot 183   CGameScriptInterface::GetNumberOfItemsOfTypeInInventory  unknown undefined()
  slot 218   CGameScriptInterface::GetNumberOfTimesHeroHasHadSex  unknown undefined()
  slot 437   CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest  __thiscall void(CGameScriptInterface *,CScriptThing *,bool)

## Logbook/UI (14)
  slot 5     CGameScriptInterface::DeactivateBoastUi         unknown undefined()
  slot 32    CGameScriptInterface::MsgIsBoastMade            __thiscall bool(CGameScriptInterface *,long)
  slot 33    CGameScriptInterface::MsgOnBoastMade            __thiscall bool(CGameScriptInterface *,long *,CCharString *)
  slot 34    CGameScriptInterface::MsgOnBoastsMade           unknown undefined()
  slot 35    CGameScriptInterface::RemoveBoastMessage        __thiscall void(CGameScriptInterface *)
  slot 303   CGameScriptInterface::AddBoast                  unknown undefined()
  slot 304   CGameScriptInterface::RemoveBoast               unknown undefined()
  slot 305   CGameScriptInterface::SetBoastAsFailed          unknown undefined()
  slot 306   CGameScriptInterface::SetBoastAsCompleted       unknown undefined()
  slot 307   CGameScriptInterface::IsBoastTaken              unknown undefined()
  slot 308   CGameScriptInterface::AddLogBookEntry           unknown undefined()
  slot 439   CGameScriptInterface::SetTrophyAsMountable      __thiscall void(CGameScriptInterface *,CScriptThing *,bool)
  slot 882   CGameScriptInterface::SetBoastingEnabled        unknown undefined()
  slot 883   CGameScriptInterface::GetBoastingEnabled        unknown undefined()

## Scene/region (5)
  slot 14    CGameScriptInterface::IsRegionDefLoaded         __thiscall bool(CGameScriptInterface *,CCharString *)
  slot 60    CGameScriptInterface::GetWanderingPopulationScriptDefNameInCurrentRegion  __thiscall void(CGameScriptInterface *,CCharString *)
  slot 61    CGameScriptInterface::GetWanderingPopulationScriptDefNameInRegion  unknown undefined()
  slot 385   CGameScriptInterface::SetCutsceneMode           __thiscall void(CGameScriptInterface *,bool,bool)
  slot 390   CGameScriptInterface::PreloadNewScene           unknown undefined()

```
