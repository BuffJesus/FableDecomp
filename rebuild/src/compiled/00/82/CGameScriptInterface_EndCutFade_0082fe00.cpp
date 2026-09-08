#include "engine/CGameScriptInterface.h"

struct CGameScriptInterface_EndCutFadeMethods : CGameScriptInterface {
    void FinishCurrentCutFade();
    void NotifyCutFadeEnded(int fadeId);
    void EndCutFade();
};

void CGameScriptInterface_EndCutFadeMethods::EndCutFade() {
    FinishCurrentCutFade();
    NotifyCutFadeEnded(0x104);
}