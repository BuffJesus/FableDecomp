#pragma once

// CQuestManager as code calls it. engine/CQuestManager.h is the generated
// flat PDB layout (a struct with no member functions); a TU that calls
// CQuestManager members includes this header instead. The two must not meet
// in one TU.
//
// The manager is reached through CTBaseSingletonNoDefaultAlloc<CQuestManager>;
// retail inlines its Get() to a load of PClass.Data at 0x013B89FC.

#include "fable_string.h"
#include "fable_system_manager.h"

template<typename T> class CArray;

class CQuestManager
{
public:
    // ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z, retail 0x004B4A10.
    // Caller: CMainGameComponent::PostInit 0x00416953 (false, true).
    bool ActivateQuest(const CCharString& questName, bool flag1, bool flag2);

    // FableWin ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z
    // (retail 0x004B4260) and ?PostBoastTransfer@CQuestManager@@QAEXXZ (retail
    // 0x004B2890); caller: CWorld::InitQuests, inlined in ActivateWorld 0x0049F180.
    bool ActivateMultipleQuests(const CArray<CCharString>& questNames, bool flag1, bool flag2);
    void PostBoastTransfer();
};
