#include <stdio.h>
#include "engine/CGameTimeManager.h"
bool __fastcall CGameTimeManager_IsDay(const CGameTimeManager*);
int main() { CGameTimeManager clock = {}; clock.Time = 0.0f; if (!CGameTimeManager_IsDay(&clock)) return 1; clock.Time = 1.0f / 6.0f; if (!CGameTimeManager_IsDay(&clock)) return 2; clock.Time = 0.5f; if (CGameTimeManager_IsDay(&clock)) return 3; clock.Time = 7.0f / 8.0f; if (CGameTimeManager_IsDay(&clock)) return 4; clock.Time = 0.9f; if (!CGameTimeManager_IsDay(&clock)) return 5; printf("GAME_TIME_IS_DAY PASS\n"); return 0; }