#include "fable_game_event_dispatch.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

static unsigned calls, errors, expectedRow;
static const CGameEvent* expectedEvent;
static CWorld* expectedWorld;
static void Mark(unsigned row,CWorld* world,const CGameEvent* event) { if (row!=expectedRow || world!=expectedWorld || event!=expectedEvent) ++errors; ++calls; }
void CWorld::EAUpdateFrame(const CGameEvent* event) { Mark(1,this,event); }
void CWorld::LEASetExclusiveMode(const CGameEvent* event) { Mark(3,this,event); }
void CWorld::LEASetDisplayMode(const CGameEvent* event) { Mark(4,this,event); }
#ifndef FABLE_WORLD_EVENT_REAL_DEFAULTS
void CWorld::LEASetEditorMode(const CGameEvent* event) { Mark(5,this,event); }
#endif
void CWorld::EAFirstPersonViewStart(const CGameEvent* event) { Mark(6,this,event); }
void CWorld::EACreatureCheat(const CGameEvent* event) { Mark(7,this,event); }
void CWorld::EAPlayerRespawn(const CGameEvent* event) { Mark(8,this,event); }
void CWorld::EASetPauseMode(const CGameEvent* event) { Mark(9,this,event); }
void CWorld::EASetSlowMotion(const CGameEvent* event) { Mark(10,this,event); }
void CWorld::EASetFreeCameraMode(const CGameEvent* event) { Mark(11,this,event); }
void CWorld::EAUseFreeCamera(const CGameEvent* event) { Mark(12,this,event); }
void CWorld::EACreatureBlock(const CGameEvent* event) { Mark(13,this,event); }
void CWorld::EATempWorldEventForWhateverYouWant(const CGameEvent* event) { Mark(14,this,event); }
#ifndef FABLE_WORLD_EVENT_REAL_DEFAULTS
void CWorld::EAApplyAllScriptedMapBrushes(const CGameEvent* event) { Mark(15,this,event); }
#endif
void CWorld::EAMoveCreature(const CGameEvent* event) { Mark(16,this,event); }
void CWorld::EACreatureUseObject(const CGameEvent* event) { Mark(17,this,event); }
void CWorld::EAControlledCreatureTalk(const CGameEvent* event) { Mark(18,this,event); }
void CWorld::EAControlledCreatureLearnExpression(const CGameEvent* event) { Mark(19,this,event); }
void CWorld::EAControlledCreatureDropObject(const CGameEvent* event) { Mark(20,this,event); }
void CWorld::EAControlledCreatureZTarget(const CGameEvent* event) { Mark(21,this,event); }
void CWorld::EAControlledCreatureUseAbility(const CGameEvent* event) { Mark(22,this,event); }
void CWorld::EAControlledCreatureStartSneaking(const CGameEvent* event) { Mark(23,this,event); }
void CWorld::EAControlledCreatureStopSneaking(const CGameEvent* event) { Mark(24,this,event); }
void CWorld::EAClickPastText(const CGameEvent* event) { Mark(25,this,event); }
void CWorld::EAOpenHeroInformationScreen(const CGameEvent* event) { Mark(26,this,event); }
void CWorld::EACloseHeroInformationScreen(const CGameEvent* event) { Mark(27,this,event); }
void CWorld::EACloseInGameMenu(const CGameEvent* event) { Mark(28,this,event); }
void CWorld::EAAnswerQuestion(const CGameEvent* event) { Mark(29,this,event); }
void CWorld::EAControlledCreatureStartThrowObject(const CGameEvent* event) { Mark(30,this,event); }
void CWorld::EAControlledCreatureEndThrowObject(const CGameEvent* event) { Mark(31,this,event); }
void CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event) { Mark(32,this,event); }
void CWorld::EAControlledCreatureUnsheatheMeleeWeapon(const CGameEvent* event) { Mark(33,this,event); }
void CWorld::EAControlledCreatureUnsheatheRangedWeapon(const CGameEvent* event) { Mark(34,this,event); }
void CWorld::EAControlledCreaturePlayerInteraction(const CGameEvent* event) { Mark(35,this,event); }
void CWorld::EAControlledCreatureGiveItemToTarget(const CGameEvent* event) { Mark(36,this,event); }
void CWorld::EAControlledCreatureUnfreezeControls(const CGameEvent* event) { Mark(37,this,event); }
void CWorld::EAControlledCreatureRoll(const CGameEvent* event) { Mark(38,this,event); }
void CWorld::EAUseQuickAccessItem(const CGameEvent* event) { Mark(39,this,event); }
void CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent* event) { Mark(40,this,event); }
void CWorld::EAChargeQuickAccessItem(const CGameEvent* event) { Mark(41,this,event); }
void CWorld::EAPutAway(const CGameEvent* event) { Mark(42,this,event); }
void CWorld::EAControlledCreatureStrafe(const CGameEvent* event) { Mark(43,this,event); }
void CWorld::EAMoveHeroToRegion(const CGameEvent* event) { Mark(44,this,event); }
void CWorld::EAFirstPersonTargeting(const CGameEvent* event) { Mark(45,this,event); }
void CWorld::EAControlledCreatureDefault(const CGameEvent* event) { Mark(46,this,event); }
void CWorld::EAExpressionFollow(const CGameEvent* event) { Mark(47,this,event); }
void CWorld::EAExpressionWait(const CGameEvent* event) { Mark(48,this,event); }
void CWorld::EAUseProjectileWeapon(const CGameEvent* event) { Mark(49,this,event); }
void CWorld::EAControlledCreatureJump(const CGameEvent* event) { Mark(50,this,event); }
void CWorld::EAMoveSpirit(const CGameEvent* event) { Mark(51,this,event); }
void CWorld::EAOpenHeroCentreDoor(const CGameEvent* event) { Mark(52,this,event); }
void CWorld::EACloseHeroCentreDoor(const CGameEvent* event) { Mark(53,this,event); }
void CWorld::EASpiritAttack(const CGameEvent* event) { Mark(54,this,event); }
void CWorld::EACreatureSprint(const CGameEvent* event) { Mark(55,this,event); }
void CWorld::EASpiritReturnToHero(const CGameEvent* event) { Mark(56,this,event); }
void CWorld::EASkipCutScene(const CGameEvent* event) { Mark(57,this,event); }
void CWorld::EAUseProjectileWeaponThirdPerson(const CGameEvent* event) { Mark(58,this,event); }
void CWorld::EAChargeUpWillSpell(const CGameEvent* event) { Mark(59,this,event); }
void CWorld::EALoadGameFromInGameMenu(const CGameEvent* event) { Mark(60,this,event); }
void CWorld::EAControlledCreatureBlockEnd(const CGameEvent* event) { Mark(61,this,event); }
void CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent* event) { Mark(62,this,event); }
void CWorld::EARemoveCurrentMode(const CGameEvent* event) { Mark(63,this,event); }
void CWorld::EAControlledCreatureLightning(const CGameEvent* event) { Mark(64,this,event); }
void CWorld::EAControlledCreatureDeactivateLightning(const CGameEvent* event) { Mark(65,this,event); }
void CWorld::EACloseLiveGUI(const CGameEvent* event) { Mark(66,this,event); }
void CWorld::EAClosePhotoCapture(const CGameEvent* event) { Mark(67,this,event); }
void CWorld::EATakeTheBloodyScreenshot(const CGameEvent* event) { Mark(68,this,event); }
void CWorld::EAConsoleToggleState(const CGameEvent* event) { Mark(69,this,event); }
void CWorld::EAOpenPCSkillsMenu(const CGameEvent* event) { Mark(70,this,event); }
void CWorld::EAOpenPCMSNChatMenu(const CGameEvent* event) { Mark(71,this,event); }
void CWorld::EAOpenPCInventoryMenu(const CGameEvent* event) { Mark(72,this,event); }
void CWorld::EAOpenPCOptionsMenu(const CGameEvent* event) { Mark(73,this,event); }
void CWorld::EAOpenPCPhotoJournalMenu(const CGameEvent* event) { Mark(74,this,event); }
void CWorld::EAOpenPCMapMenu(const CGameEvent* event) { Mark(75,this,event); }
void CWorld::EAOpenPCBuyTradingMenu(const CGameEvent* event) { Mark(76,this,event); }
void CWorld::EAOpenPCSellTradingMenu(const CGameEvent* event) { Mark(77,this,event); }
void CWorld::EAOpenPCWantedTradingMenu(const CGameEvent* event) { Mark(78,this,event); }
void CWorld::EAOpenPCStatusMenu(const CGameEvent* event) { Mark(79,this,event); }
void CWorld::EAOpenPCScoreboard(const CGameEvent* event) { Mark(80,this,event); }
void CWorld::EADeleteMenuComponents(const CGameEvent* event) { Mark(81,this,event); }
void CWorld::EATogglePassiveAggressiveMode(const CGameEvent* event) { Mark(82,this,event); }
void CWorld::EADeleteTradeActiveMenu(const CGameEvent* event) { Mark(83,this,event); }
void CWorld::EAToggleKillEverythingMode(const CGameEvent* event) { Mark(84,this,event); }
void CWorld::EAToggleViewHeroMode(const CGameEvent* event) { Mark(85,this,event); }
void CWorld::EACentreCamera(const CGameEvent* event) { Mark(86,this,event); }
struct CGameEventDispatchTestAccess { static const CGameEventDispatch* Get() { return CMainGameComponent::EventDispatch; } };

int main() {
    const CGameEventDispatch* table=CGameEventDispatchTestAccess::Get();
    unsigned scenarios=0;
    for (unsigned pattern=0; pattern<64; ++pattern) {
        unsigned long worldStorage[104]; memset(worldStorage,pattern,sizeof(worldStorage));
        CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);
        unsigned long before[104]; memcpy(before,worldStorage,sizeof(before));
        for (unsigned row=0; row<87; ++row) {
            CGameEvent event(row); expectedRow=row;expectedEvent=&event;expectedWorld=world;
            for (unsigned field=0;field<2;++field) {
                CGameEventDispatch::Action action=field?table[row].LocalActionFunction:table[row].ActionFunction;
                const unsigned long* words=reinterpret_cast<const unsigned long*>(&action);
                const bool active=field?(row>=3 && row<=5):(row!=0 && row!=2 && !(row>=3 && row<=5));
                if (bool(action)!=active || words[1] || words[2] || words[3]!=(active?0:0xffffffffUL)) return 1;
                calls=errors=0;if (action) (world->*action)(&event);
                unsigned expectedCalls=unsigned(active);
#ifdef FABLE_WORLD_EVENT_REAL_DEFAULTS
                if (row==5 || row==15) expectedCalls=0;
#endif
                if (calls!=expectedCalls || errors || memcmp(before,worldStorage,sizeof(before))) return 2;
                ++scenarios;
            }
            CGameEventDispatch::Feedback feedback[2]={table[row].FeedbackFunction,table[row].ConfirmFunction};
            for (unsigned field=0;field<2;++field) {
                const unsigned long* words=reinterpret_cast<const unsigned long*>(&feedback[field]);
                if (feedback[field] || words[0] || words[1] || words[2] || words[3]!=0xffffffffUL) return 3;
                ++scenarios;
            }
        }
    }
    printf("DISPATCH_TABLE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
