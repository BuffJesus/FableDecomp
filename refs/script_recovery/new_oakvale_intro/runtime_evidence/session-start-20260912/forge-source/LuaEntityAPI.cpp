#include "LuaEntityAPI.h"
#include "EntityScriptingAPI.h" 
#include "GameInterface.h"
#include "FableAPI.h"
#include "LuaEntityHost.h"
#include "LuaQuestHost.h"
#include <windows.h>
#include <sstream>
#include <unordered_map>

namespace {
std::unordered_map<void*, CScriptGameResourceObjectScriptedThingBase*> g_controlHandlesByEntityData;
}

CScriptGameResourceObjectScriptedThingBase* LuaEntityAPI::FindControlHandle(CScriptThing* pThing) {
    if (!pThing || !pThing->pImp.Data) return nullptr;
    auto it = g_controlHandlesByEntityData.find(pThing->pImp.Data);
    return it == g_controlHandlesByEntityData.end() ? nullptr : it->second;
}

void LuaEntityAPI::SelectControlHandle(CScriptThing* pThing) {
    m_pControlHandle = nullptr;
    m_pControlledEntityData = nullptr;
    if (!pThing || !pThing->pImp.Data) return;

    // Entity Lua VMs are isolated, but a script may also acquire temporary
    // control of Hero or another NPC. Always select the handle owned by this
    // LuaEntityAPI instance. The process-wide registry exists only so quest
    // cutscene assembly can borrow any live handle for an actor; using it here
    // could make one VM operate through, or release, another VM's resource.
    auto owned = m_ownedControlHandles.find(pThing->pImp.Data);
    if (owned != m_ownedControlHandles.end()) {
        m_pControlHandle = owned->second;
        m_pControlledEntityData = pThing->pImp.Data;
        return;
    }
    auto borrowed = m_borrowedControlHandles.find(pThing->pImp.Data);
    if (borrowed == m_borrowedControlHandles.end()) return;
    m_pControlHandle = borrowed->second;
    m_pControlledEntityData = pThing->pImp.Data;
}

LuaEntityAPI::~LuaEntityAPI() {
    if (!CSGROSTB_Destroy_API || !Game_free) return;
    for (const auto& entry : m_ownedControlHandles) {
        auto global = g_controlHandlesByEntityData.find(entry.first);
        if (global != g_controlHandlesByEntityData.end() && global->second == entry.second) {
            g_controlHandlesByEntityData.erase(global);
        }
        CSGROSTB_Destroy_API(entry.second);
        Game_free(entry.second);
    }
    m_ownedControlHandles.clear();
    m_ownedControlPriorities.clear();
    m_ownedControlDepth.clear();
    m_borrowedControlHandles.clear();
    m_pControlHandle = nullptr;
    m_pControlledEntityData = nullptr;
}

CScriptGameResourceObjectScriptedThingVTable* LuaEntityAPI::GetExpertVTable(CScriptThing* pMe) {
    // Basic null checks
    if (!pMe) {
        // LogToFile("!!! GetExpertVTable Error: pMe is NULL!"); // Optional detailed log
        return nullptr;
    }
    if (!pMe->pImp.Data) {
        std::stringstream ss_log;
        ss_log << "!!! GetExpertVTable Error: pMe (0x" << pMe << ")->pImp.Data is NULL!";
        LogToFile(ss_log.str());
        return nullptr;
    }
    // Get the implementation object (expert)
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(pMe->pImp.Data);
    if (!pExpert->pVTable) {
        std::stringstream ss_log;
        ss_log << "!!! GetExpertVTable Error: pExpert (0x" << pExpert << ")->pVTable is NULL!";
        LogToFile(ss_log.str());
        return nullptr;
    }
    // Return the VTable pointer, cast to our struct type
    return reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable);
}

void LuaEntityAPI::SetGameInterface(CGameScriptInterfaceBase* pInterface)
{
    m_pGameInterface = pInterface;
}

void LuaEntityAPI::SetThreadHost(LuaQuestHost* pQuestHost, LuaEntityHost* pEntityHost)
{
    m_pQuestHost = pQuestHost;
    m_pEntityHost = pEntityHost;
}

bool LuaEntityAPI::IsThreadTerminating() const
{
    if (m_pEntityHost && IsActiveThreadTerminating_Entity_API) {
        return IsActiveThreadTerminating_Entity_API(m_pEntityHost);
    }
    if (m_pQuestHost && IsActiveThreadTerminating_Quest_API) {
        return IsActiveThreadTerminating_Quest_API(&m_pQuestHost->base);
    }
    return false;
}

bool LuaEntityAPI::AcquireControl(CScriptThing* pMe, sol::optional<int> priority) {
    SelectControlHandle(pMe);
    void* const entityData = (pMe && pMe->pImp.Data) ? pMe->pImp.Data : nullptr;
    const int requestedPriority = priority.value_or(static_cast<int>(SCRIPT_AI_PRIORITY_HIGHEST));
    if (m_pControlHandle) {
        const auto borrowed = entityData
            ? m_borrowedControlHandles.find(entityData)
            : m_borrowedControlHandles.end();
        if (borrowed != m_borrowedControlHandles.end() && borrowed->second == m_pControlHandle) {
            return true;
        }
        // Retail entity scripts nest a second StartScriptingEntity resource (for
        // example priority-4 conversation control over priority-3 Main control)
        // inside ONE native script owner, and the scheduler grants it. A Forge
        // handle carries no such owner identity: a second resource for an actor this
        // VM already controls is treated as a foreign request and is never granted
        // while the outer resource is live (runtime evidence
        // interactive-20260911-v10-wife-nested-acquire-hang.log: Affair Wife waited
        // >600 frames at priority 4 over her own priority-3 Main handle). The
        // observable retail contract is that the outer resource survives the nested
        // release, so nested acquires reuse the live handle and are depth-counted;
        // only the outermost ReleaseControl destroys it.
        // Retail Main loops also call StartScriptingEntity again on the SAME resource
        // object at the top of every iteration (Bully, Victim, Barrel Man), which the
        // engine treats as idempotent; v11 counted every such call and depth inflated
        // past 400. Lua cannot name the resource object, so a re-acquire at the
        // priority already held saturates at one level: the outer handle must still
        // survive a following release (Affair Man's conversation release precedes his
        // walk-home loop, proven clean in the v11 run), but repeats add nothing.
        // A different priority is a distinct nested resource and counts one level.
        const auto heldPriority = m_ownedControlPriorities.find(entityData);
        int& depthSlot = m_ownedControlDepth[entityData];
        if (heldPriority != m_ownedControlPriorities.end() && heldPriority->second == requestedPriority && depthSlot > 0) {
            return true;
        }
        const int depth = ++depthSlot;
        LogToFile("    [LuaEntityAPI::AcquireControl] Reusing live control handle for nested acquire (depth " +
            std::to_string(depth) + ", requested priority " + std::to_string(requestedPriority) + ").");
        return true;
    }
    else if (entityData) {
        // Native scripts can nest StartScriptingEntity resources for an actor whose
        // own script is still active. An isolated Forge Lua VM cannot acquire that
        // actor a second time through the scheduler, so borrow the process-wide live
        // resource and preserve its ownership. This is required by the retail Bully
        // run-off path, where the Victim waits on BullyRanOff while Bully controls him
        // for the cutscene. Borrowing never applies to a resource this VM owns itself.
        auto live = g_controlHandlesByEntityData.find(pMe->pImp.Data);
        if (live != g_controlHandlesByEntityData.end() && live->second && live->second->pImp.Data) {
            m_borrowedControlHandles[pMe->pImp.Data] = live->second;
            m_pControlHandle = live->second;
            m_pControlledEntityData = pMe->pImp.Data;
            LogToFile("    [LuaEntityAPI::AcquireControl] Borrowed another Lua VM's live control handle.");
            return true;
        }
    }

    const bool hasTerminationAPI = (m_pEntityHost && IsActiveThreadTerminating_Entity_API) ||
        (m_pQuestHost && IsActiveThreadTerminating_Quest_API);
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !hasTerminationAPI ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API || !Game_malloc ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable ||
        !InitScriptObjectHelper1_API || !InitScriptObjectHelper2_API)
    {
        LogToFile("!!! ERROR: AcquireControl - one or more required game API functions are null!");
        return false;
    }

    m_pControlHandle = static_cast<CScriptGameResourceObjectScriptedThingBase*>(Game_malloc(sizeof(CScriptGameResourceObjectScriptedThingBase)));
    if (!m_pControlHandle) {
        LogToFile("!!! ERROR: AcquireControl - Failed to malloc handle!");
        return false;
    }

    CBaseObject_Construct_API(m_pControlHandle);
    m_pControlHandle->pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    m_pControlHandle->pImp.Data = nullptr;
    m_pControlHandle->pImp.Info = nullptr;

    LogToFile("    [LuaEntityAPI] Initializing handle with Helpers...");
    if (InitScriptObjectHelper1_API(m_pControlHandle)) {
        InitScriptObjectHelper2_API(m_pControlHandle);
        LogToFile("    [LuaEntityAPI] ...Helper initialization completed.");
    }
    else {
        LogToFile("    [LuaEntityAPI] ...Helper1 returned false, skipped Helper2.");
    }

    const EScriptAIPriority scriptPriority = static_cast<EScriptAIPriority>(requestedPriority);
    LogToFile("    [LuaEntityAPI] Acquiring script control for entity at priority " +
        std::to_string(static_cast<int>(scriptPriority)) + "...");
    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, m_pControlHandle, scriptPriority)) {
            m_pControlledEntityData = pMe->pImp.Data;
            if (m_pControlledEntityData) {
                g_controlHandlesByEntityData[m_pControlledEntityData] = m_pControlHandle;
                m_ownedControlHandles[m_pControlledEntityData] = m_pControlHandle;
                m_ownedControlPriorities[m_pControlledEntityData] = static_cast<int>(scriptPriority);
                m_ownedControlDepth[m_pControlledEntityData] = 0;
            }
            LogToFile("    [LuaEntityAPI] ...Control acquired successfully.");
            return true;
        }
        NewScriptFrame_API(m_pGameInterface);

        if (IsThreadTerminating()) {
            LogToFile("    [LuaEntityAPI] ...Thread terminating during control acquisition.");

            CSGROSTB_Destroy_API(m_pControlHandle);
            Game_free(m_pControlHandle);
            m_pControlHandle = nullptr;

            return false;
        }
    }
}

void LuaEntityAPI::ReleaseControl(CScriptThing* pMe) {
    SelectControlHandle(pMe);
    if (!m_pControlHandle) {
        LogToFile("    [LuaEntityAPI::ReleaseControl] Warning: No handle to release.");
        return;
    }

    if (!CSGROSTB_Destroy_API || !Game_free) {
        LogToFile("!!! ERROR: ReleaseControl - Destructor or Game_free API is null!");
        return;
    }

    LogToFile("    [LuaEntityAPI] Releasing script control handle...");

    auto borrowed = m_borrowedControlHandles.find(m_pControlledEntityData);
    if (borrowed != m_borrowedControlHandles.end() && borrowed->second == m_pControlHandle) {
        m_borrowedControlHandles.erase(borrowed);
        m_pControlHandle = nullptr;
        m_pControlledEntityData = nullptr;
        LogToFile("    [LuaEntityAPI] Borrowed control handle released without destroying its owner's resource.");
        return;
    }

    // A nested release only unwinds one reuse level; the outer resource stays live
    // exactly as the retail outer StartScriptingEntity resource does.
    auto depth = m_ownedControlDepth.find(m_pControlledEntityData);
    if (depth != m_ownedControlDepth.end() && depth->second > 0) {
        --depth->second;
        LogToFile("    [LuaEntityAPI] Nested release: outer control handle retained (depth " +
            std::to_string(depth->second) + ").");
        return;
    }

    void* releasedEntityData = m_pControlledEntityData;
    if (releasedEntityData) {
        auto it = g_controlHandlesByEntityData.find(releasedEntityData);
        if (it != g_controlHandlesByEntityData.end() && it->second == m_pControlHandle) {
            g_controlHandlesByEntityData.erase(it);
        }
        m_ownedControlHandles.erase(releasedEntityData);
        m_ownedControlPriorities.erase(releasedEntityData);
        m_ownedControlDepth.erase(releasedEntityData);
        m_pControlledEntityData = nullptr;
    }

    CSGROSTB_Destroy_API(m_pControlHandle);

    Game_free(m_pControlHandle);

    m_pControlHandle = nullptr;
}

void LuaEntityAPI::MakeBehavioral(CScriptThing* pMe) {
    if (!m_pGameInterface || !EntityAttachToScript_API || !GetActiveQuestName_API) {
        LogToFile("!!! ERROR: MakeBehavioral - required API functions not initialized!");
        return;
    }

    CCharString questName = { 0 };
    GetActiveQuestName_API(m_pGameInterface, &questName);

    if (questName.pStringData) {
        const char* text = CCharString_ToConstChar_API(&questName);
        if (text) {
            LogToFile("   [DIAGNOSTIC] GetActiveQuestName returned: '" + std::string(text) + "'. Attaching entity...");
            EntityAttachToScript_API(m_pGameInterface, pMe, &questName);
            LogToFile("   ...Entity attached successfully.");
        }
        else {
            LogToFile("   [DIAGNOSTIC] GetActiveQuestName returned a valid CCharString but the text was null.");
        }
        CCharString_Destroy(&questName);
    }
    else {
        LogToFile("   [DIAGNOSTIC] GetActiveQuestName returned a NULL CCharString. Cannot attach entity.");
    }
}

bool LuaEntityAPI::IsTalkedToByHero(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsTalkedToBy) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsTalkedToBy(pMe, heroName.get());
}

void LuaEntityAPI::SpeakAndWait(CScriptThing* pMe, const std::string& dialogueKey, int selectionMethod) {
    SelectControlHandle(pMe);
    LogToFile("--- SpeakAndWait (Using Pre-Acquired Handle): INITIATED for entity '" + GetDataString(pMe) + "' ---");

    if (!m_pGameInterface || !StartMovieSequence_API || !PauseAllNonScriptedEntities_API || !GetHero_API ||
        !CBaseObject_Construct_API || !MovieResource_Destroy_API || !g_pMovieObjectVTable ||
        !NewScriptFrame_API || !IsActiveThreadTerminating_API) {
        LogToFile("!!! ERROR: SpeakAndWait - one or more required game API functions are null!");
        return;
    }

    if (!m_pControlHandle) {
        LogToFile("!!! ERROR: SpeakAndWait - No control handle! Did you call AcquireControl()?");
        return;
    }

    CScriptGameResourceObjectMovieBase speech_movie;
    bool movieStarted = false;
    bool worldPaused = false;

    do
    {

        LogToFile("    [SpeakAndWait] Starting cinematic mode...");
        CBaseObject_Construct_API(&speech_movie);
        speech_movie.pVTable = g_pMovieObjectVTable;
        speech_movie.pImp.Data = nullptr;
        speech_movie.pImp.Info = nullptr;
        movieStarted = true;

        FableString classStr("Class");
        StartMovieSequence_API(m_pGameInterface, classStr, &speech_movie);
        PauseAllNonScriptedEntities_API(m_pGameInterface, true);
        worldPaused = true;
        LogToFile("    [SpeakAndWait] ...Cinematic mode started and world is paused.");

        LogToFile("    [SpeakAndWait] Getting Expert from handle...");
        CScriptGameResourceObjectScriptedThing* pExpert = (CScriptGameResourceObjectScriptedThing*)m_pControlHandle->pImp.Data;
        if (!pExpert || !pExpert->pVTable) {
            LogToFile("!!! ERROR: SpeakAndWait - Expert script object (pExpert) or its VTable is null!");
            break;
        }

        CScriptGameResourceObjectScriptedThingVTable* pVTable = (CScriptGameResourceObjectScriptedThingVTable*)pExpert->pVTable;
        LogToFile("    [SpeakAndWait] ...Got Expert successfully.");

        if (pVTable->ClearCommands) {
            LogToFile("    [SpeakAndWait] Clearing existing commands (movement, etc.)...");
            try {
                pVTable->ClearCommands(pExpert);
                LogToFile("    [SpeakAndWait] ...Commands cleared successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during ClearCommands in SpeakAndWait!");
            }
        }
        else {
            LogToFile("!!! WARNING: ClearCommands function pointer is null!");
        }

        LogToFile("    [SpeakAndWait] Issuing 'Speak' command...");
        CScriptThing* pHero = GetHero_API(m_pGameInterface);
        if (!pHero) {
            LogToFile("!!! WARNING: SpeakAndWait - GetHero_API returned null!");
        }

        if (!pVTable->Speak) {
            LogToFile("!!! ERROR: SpeakAndWait - Speak function pointer is null!");
            break;
        }

        try {
            pVTable->Speak(pExpert, pHero, dialogueKey.c_str(),
                static_cast<ETextGroupSelectionMethod>(selectionMethod),
                false, true, false);
            LogToFile("    [SpeakAndWait] ...'Speak' command issued successfully.");
        }
        catch (...) {
            LogToFile("!!! EXCEPTION during Speak call in SpeakAndWait!");
            break;
        }

        if (!pVTable->IsPerformingScriptTask) {
            LogToFile("!!! ERROR: SpeakAndWait - IsPerformingScriptTask function pointer is null!");
            break;
        }

        LogToFile("    [SpeakAndWait] Waiting for speech completion...");
        int waitFrames = 0;
        while (pExpert && pVTable->IsPerformingScriptTask(pExpert)) {
            NewScriptFrame_API(m_pGameInterface);
            waitFrames++;

            if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
                LogToFile("    [SpeakAndWait] ...Thread terminating during speech wait (waited " + std::to_string(waitFrames) + " frames).");
                break;
            }
        }
        LogToFile("    [SpeakAndWait] ...'Speak' command has completed after " + std::to_string(waitFrames) + " frames.");

    } while (false);

    LogToFile("    [SpeakAndWait] Starting cleanup...");
    if (worldPaused) {
        PauseAllNonScriptedEntities_API(m_pGameInterface, false);
        LogToFile("    [SpeakAndWait] ...World unpaused.");
    }
    if (movieStarted) {
        MovieResource_Destroy_API(&speech_movie);
        LogToFile("    [SpeakAndWait] ...Movie resource object destroyed.");
    }

    LogToFile("    [SpeakAndWait] Handle NOT released (managed by AcquireControl/ReleaseControl).");

    LogToFile("--- SpeakAndWait (Using Pre-Acquired Handle): COMPLETED ---");
}

void LuaEntityAPI::GainControlAndSpeak(CScriptThing* pMe, const std::string& dialogueKey, int selectionMethod) {
    LogToFile("--- SpeakAndWait: INITIATED for entity '" + GetDataString(pMe) + "' ---");

    if (!m_pGameInterface || !StartScriptingEntity_API || !StartMovieSequence_API || !PauseAllNonScriptedEntities_API || !GetHero_API || !InitScriptObjectHelper1_API || !InitScriptObjectHelper2_API || !CBaseObject_Construct_API || !MovieResource_Destroy_API || !CSGROSTB_Destroy_API) {
        LogToFile("!!! ERROR: SpeakAndWait - one or more required game API functions are null!");
        return;
    }

    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptGameResourceObjectMovieBase speech_movie;
    bool controlAcquired = false;
    bool movieStarted = false;
    bool worldPaused = false;

    do
    {
        LogToFile("    [SpeakAndWait] STEP 1: Preparing script control handle (seh_me)...");
        CBaseObject_Construct_API(&seh_me);
        seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
        seh_me.pImp.Data = nullptr;
        seh_me.pImp.Info = nullptr;

        if (InitScriptObjectHelper1_API(&seh_me)) {
            InitScriptObjectHelper2_API(&seh_me);
        }

        LogToFile("    [SpeakAndWait] STEP 2: Attempting to acquire script control...");
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            LogToFile("    [SpeakAndWait] ...Control acquired successfully on first attempt.");
        }
        else {
            LogToFile("    [SpeakAndWait] ...Initial attempt failed. Entering wait loop...");
            while (true) {
                NewScriptFrame_API(m_pGameInterface);
                if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
                    LogToFile("    [SpeakAndWait] ...Thread is terminating. Aborting control acquisition.");
                    break;
                }
                if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    controlAcquired = true;
                    LogToFile("    [SpeakAndWait] ...Control acquired successfully within wait loop.");
                    break;
                }
            }
        }

        if (!controlAcquired) {
            LogToFile("!!! ERROR: SpeakAndWait - Failed to acquire script control of entity. Aborting function.");
            break;
        }

        LogToFile("    [SpeakAndWait] STEP 3 & 4: Starting cinematic mode...");
        CBaseObject_Construct_API(&speech_movie);
        speech_movie.pVTable = g_pMovieObjectVTable;
        speech_movie.pImp.Data = nullptr;
        speech_movie.pImp.Info = nullptr;
        movieStarted = true;
        FableString classStr("Class");
        StartMovieSequence_API(m_pGameInterface, classStr, &speech_movie);
        PauseAllNonScriptedEntities_API(m_pGameInterface, true);
        worldPaused = true;
        LogToFile("    [SpeakAndWait] ...Cinematic mode started and world is paused.");

        LogToFile("    [SpeakAndWait] STEP 5 & 6: Issuing 'Speak' command...");
        CScriptGameResourceObjectScriptedThing* pExpert = (CScriptGameResourceObjectScriptedThing*)seh_me.pImp.Data;
        if (pExpert && pExpert->pVTable) {
            CScriptGameResourceObjectScriptedThingVTable* pVTable = (CScriptGameResourceObjectScriptedThingVTable*)pExpert->pVTable;
            CScriptThing* pHero = GetHero_API(m_pGameInterface);
            pVTable->Speak(pExpert, pHero, dialogueKey.c_str(), static_cast<ETextGroupSelectionMethod>(selectionMethod), false, true, false);
            LogToFile("    [SpeakAndWait] ...'Speak' command issued. Waiting for completion...");
            while (pExpert && pVTable->IsPerformingScriptTask(pExpert)) {
                NewScriptFrame_API(m_pGameInterface);
                if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
            }
            LogToFile("    [SpeakAndWait] ...'Speak' command has completed.");
        }
        else {
            LogToFile("!!! ERROR: SpeakAndWait - Expert script object (pExpert) or its VTable is null!");
        }

    } while (false);

    LogToFile("    [SpeakAndWait] STEP 7: Starting cleanup...");
    if (worldPaused) {
        PauseAllNonScriptedEntities_API(m_pGameInterface, false);
        LogToFile("    [SpeakAndWait] ...World unpaused.");
    }
    if (movieStarted) {
        MovieResource_Destroy_API(&speech_movie);
        LogToFile("    [SpeakAndWait] ...Movie resource object destroyed.");
    }

    if (controlAcquired) {
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [SpeakAndWait] ...Script control handle destroyed.");
    }

    LogToFile("--- SpeakAndWait: COMPLETED ---");
}

bool LuaEntityAPI::IsNull(CScriptThing* pMe) {
    if (!pMe) return true;

    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return true;

    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    return pVTable->IsNull(pImp);
}

sol::table LuaEntityAPI::GetPos(CScriptThing* pMe, sol::this_state s) {
    sol::state_view lua(s);
    sol::table pos_table = lua.create_table();

    if (!pMe) return pos_table;

    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return pos_table;

    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    const C3DVector* pPos = pVTable->GetPos(pImp);

    if (pPos) {
        pos_table["x"] = pPos->x;
        pos_table["y"] = pPos->y;
        pos_table["z"] = pPos->z;
    }

    return pos_table;
}

bool LuaEntityAPI::IsDistanceFromPositionOver(CScriptThing* pMe, sol::table position, float distance) {
    if (!pMe) {
        return false;
    }

    // Retail helper 0x00CBE45C has the complete prototype
    // __fastcall bool(CScriptThing*, const C3DVector*, float). Calling it directly
    // preserves the native float arithmetic, validation, and strict boundary test.
    using tIsDistanceFromThingToPositionOver =
        bool(__fastcall*)(CScriptThing*, const C3DVector*, float);
    tIsDistanceFromThingToPositionOver retailDistance =
        ASLR<tIsDistanceFromThingToPositionOver>(0x00CBE45C);
    const C3DVector target = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };
    return retailDistance(pMe, &target, distance);
}

float LuaEntityAPI::GetAngleXY(CScriptThing* pMe) {
    if (!pMe) return 0.0f;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return 0.0f;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);
    return pVTable->GetAngleXY ? pVTable->GetAngleXY(pImp) : 0.0f;
}

void LuaEntityAPI::SetIsPushableByHero(CScriptThing* pMe, bool isPushable) {
    if (!m_pGameInterface || !pMe || !SetIsPushableByHero_API) {
        LogToFile("!!! SetIsPushableByHero(entity): API or entity unavailable");
        return;
    }
    CScriptThing retainedCopy = *pMe;
    if (retainedCopy.pImp.Info) {
        retainedCopy.pImp.Info->RefCount++;
    }
    SetIsPushableByHero_API(m_pGameInterface, retainedCopy, isPushable);
}

std::string LuaEntityAPI::GetDataString(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return "";
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;
    CCharString result = { 0 };
    pVTable->GetDataString(pMe, &result);
    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
    }
    return "";
}

void LuaEntityAPI::SetReadableText(CScriptThing* pMe, const std::string& textTag)
{
    if (!m_pGameInterface || !pMe || !SetReadableObjectTextTag_API)
    {
        return;
    }
    FableString fsTextTag(textTag.c_str());
    SetReadableObjectTextTag_API(m_pGameInterface, pMe, fsTextTag);
}

bool LuaEntityAPI::IsKilledByHero(CScriptThing* pMe) {
    if (!pMe) return false;

    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);

    if (!pImp || !pImp->pVTable) return false;

    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    FableString heroName("First2");

    bool result = pVTable->MsgIsKilledBy(pImp, heroName);

    if (result) {
        LogToFile("    [ENTITY_API] IsKilledByHero check returned TRUE for entity '" + GetDataString(pMe) + "'.");
    }

    return result;
}

bool LuaEntityAPI::MsgIsPresentedWithItem(CScriptThing* pMe, sol::this_state s) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsPresentedWithItem) return false;

    CCharString result = { 0 };
    bool wasPresented = pVTable->MsgIsPresentedWithItem(pMe, &result);
    if (wasPresented) {
        sol::state_view lua(s);
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            lua.set("g_PresentedItemName", std::string(text));
        }
        CCharString_Destroy(&result);
    }
    return wasPresented;
}

bool LuaEntityAPI::MsgIsKilledBy(CScriptThing* pMe, const std::string& killerName) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);
    if (!pVTable->MsgIsKilledBy) {
        LogToFile("!!! ERROR: MsgIsKilledBy - function pointer is null in CGameScriptThing VTable!");
        return false;
    }
    FableString fsKillerName(killerName.c_str());
    bool result = pVTable->MsgIsKilledBy(pImp, fsKillerName.get());
    if (result) {
        LogToFile("    [ENTITY_API] MsgIsKilledBy check returned TRUE for entity '" + GetDataString(pMe) + "' and killer '" + killerName + "'.");
    }
    return result;
}

bool LuaEntityAPI::MsgIsHitByHero(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsHitBy) return false;
    FableString heroName("SCRIPT_NAME_HERO");
    const bool hit = pVTable->MsgIsHitBy(pMe, heroName.get());
    if (hit) {
        LogToFile("    [HitEvidence] MsgIsHitBy(SCRIPT_NAME_HERO) returned true.");
    }
    return hit;
}

bool LuaEntityAPI::MsgIsHitByAnySpecialAbilityFromHero(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsHitByAnySpecialAbilityFrom) return false;
    FableString heroName("SCRIPT_NAME_HERO");
    const bool hit = pVTable->MsgIsHitByAnySpecialAbilityFrom(pMe, heroName.get());
    if (hit) {
        LogToFile("    [HitEvidence] MsgIsHitByAnySpecialAbilityFrom(SCRIPT_NAME_HERO) returned true.");
    }
    return hit;
}

bool LuaEntityAPI::MsgIsHitByHealLifeFromHero(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsHitBySpecialAbilityFrom) return false;
    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsHitBySpecialAbilityFrom(pMe, HERO_ABILITY_HEAL_LIFE_SPELL, heroName.get());
}

bool LuaEntityAPI::IsAwareOfHero(CScriptThing* pMe) {
    if (!pMe) return false;
    // Get implementation pointer and vtable
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    // Check function pointer validity
    if (!pVTable->IsAwareOfHero) {
        // LogToFile("!!! ERROR: IsAwareOfHero - function pointer is null in VTable!"); // Optional log
        return false;
    }
    // Call the function
    return pVTable->IsAwareOfHero(pImp);
}

bool LuaEntityAPI::IsPerformingScriptTask(CScriptThing* pMe) {
    SelectControlHandle(pMe);
    // 1. Check if we have the handle from AcquireControl()
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        // LogToFile("!!! ERROR: IsPerformingScriptTask - No control handle! Did you call AcquireControl()?");
        // This is not an error. If we have no handle, we are not performing a script task.
        return false;
    }

    // 2. Get the Expert object and VTable from the *existing* handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    // 3. Check VTable and the specific function pointer
    if (!pVTable || !pVTable->IsPerformingScriptTask) {
        LogToFile("!!! ERROR: IsPerformingScriptTask function pointer invalid!");
        return false;
    }

    // 4. Call the function using the handle's implementation pointer (pExpert)
    try {
        return pVTable->IsPerformingScriptTask(pExpert);
    }
    catch (...) {
        LogToFile("!!! EXCEPTION during IsPerformingScriptTask call!");
        return false;
    }
}

sol::object LuaEntityAPI::MsgExpressionPerformedTo(CScriptThing* pMe, sol::this_state s) {
    sol::state_view lua(s);
    if (!pMe || !pMe->pVTable) return sol::make_object(lua, sol::nil);
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);

    // Check function pointer validity
    if (!pVTable->MsgExpressionPerformedTo || !CCharString_ToConstChar_API || !CCharString_Destroy) {
        // LogToFile("!!! ERROR: MsgExpressionPerformedTo prerequisites failed!"); // Optional log
        return sol::make_object(lua, sol::nil);
    }

    CCharString expressionNameResult = { 0 }; // Output parameter
    bool messageTriggered = pVTable->MsgExpressionPerformedTo(pMe, &expressionNameResult);

    if (messageTriggered && expressionNameResult.pStringData) {
        const char* text = CCharString_ToConstChar_API(&expressionNameResult);
        if (text) {
            std::string name(text);
            CCharString_Destroy(&expressionNameResult);
            return sol::make_object(lua, name); // Return expression name string
        }
        CCharString_Destroy(&expressionNameResult); // Destroy even if conversion failed
    }

    // Message not triggered or string conversion failed
    if (expressionNameResult.pStringData) CCharString_Destroy(&expressionNameResult); // Ensure cleanup if message was false but string was allocated
    return sol::make_object(lua, sol::nil); // Return nil
}

int LuaEntityAPI::MsgHowLongWasExpressionPerformed(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return 0;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);

    // Check function pointer validity
    if (!pVTable->MsgHowLongWasExpressionPerformed) {
        // LogToFile("!!! ERROR: MsgHowLongWasExpressionPerformed - function pointer is null in VTable!"); // Optional log
        return 0;
    }
    // Call the function
    return pVTable->MsgHowLongWasExpressionPerformed(pMe);
}

sol::object LuaEntityAPI::MsgReceivedMoney(CScriptThing* pMe, sol::this_state s) {
    sol::state_view lua(s);
    if (!pMe || !pMe->pVTable) return sol::make_object(lua, sol::nil);
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);

    // Check function pointer validity
    if (!pVTable->MsgReceivedMoney) {
        // LogToFile("!!! ERROR: MsgReceivedMoney - function pointer is null in VTable!"); // Optional log
        return sol::make_object(lua, sol::nil);
    }

    int amountReceived = 0; // Output parameter
    bool messageTriggered = pVTable->MsgReceivedMoney(pMe, &amountReceived);

    if (messageTriggered) {
        return sol::make_object(lua, amountReceived); // Return the amount
    }
    else {
        return sol::make_object(lua, sol::nil); // Return nil if message not triggered
    }
}

void LuaEntityAPI::MoveToPosition(CScriptThing* pMe, sol::table position, float radius, int moveType) {
    LogToFile("===== MoveToPosition Wrapper (With Wait Loop) START =====");
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    {
        LogToFile("!!! ERROR: MoveToPosition - one or more required game API functions are null!");
        LogToFile("===== MoveToPosition Wrapper END (Failure: API Missing) =====");
        return;
    }

    // --- Input Validation ---
    if (!pMe) {
        LogToFile("!!! ERROR: MoveToPosition - pMe is NULL!");
        return;
    }
    if (!pMe->pImp.Data) {
        LogToFile("!!! ERROR: MoveToPosition - pMe->pImp.Data is NULL!");
        return;
    }

    // Extract position from Lua table
    sol::optional<float> optX = position["x"];
    sol::optional<float> optY = position["y"];
    sol::optional<float> optZ = position["z"];
    C3DVector pos = {
        optX ? *optX : 0.0f,
        optY ? *optY : 0.0f,
        optZ ? *optZ : 0.0f
    };

    ss_log << "    Target position: x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z
        << ", radius=" << radius << ", moveType=" << moveType;
    LogToFile(ss_log.str());

    // --- Acquire Script Control ---
    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;

    LogToFile("    [MoveToPosition] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr;
    seh_me.pImp.Info = nullptr;

    LogToFile("    [MoveToPosition] STEP 2: Attempting to acquire script control...");
    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            LogToFile("    [MoveToPosition] ...Control acquired successfully.");
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [MoveToPosition] ...Thread is terminating during control acquisition. Aborting.");
            break;
        }
    }

    // --- If Control Acquired, Issue Command and Wait ---
    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
            reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // --- Clear Existing Commands ---
            if (pVTable->ClearCommands) {
                LogToFile("    [MoveToPosition] STEP 3: Clearing existing commands...");
                try {
                    pVTable->ClearCommands(pExpert);
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during ClearCommands call!");
                }
            }
            else {
                LogToFile("!!! WARNING: ClearCommands function pointer is null!");
            }

            // --- Issue MoveToPosition Command ---
            if (pVTable->MoveToPosition) {
                LogToFile("    [MoveToPosition] STEP 4: Issuing MoveToPosition command...");
                EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);

                // Parameters: b1 = avoid_obstacles (true), b2 = ignore_path_preferability (false)
                bool bAvoidObstacles = true;
                bool bIgnorePathPref = false;

                ss_log.str("");
                ss_log << "  Parameters for API call:"
                    << "\n    this (pExpert): 0x" << std::hex << pExpert
                    << "\n    position: (" << std::dec << pos.x << ", " << pos.y << ", " << pos.z << ")"
                    << "\n    radius: " << radius
                    << "\n    moveType: " << moveType
                    << "\n    avoid_obstacles: " << (bAvoidObstacles ? "true" : "false")
                    << "\n    ignore_path_pref: " << (bIgnorePathPref ? "true" : "false");
                LogToFile(ss_log.str());

                try {
                    pVTable->MoveToPosition(pExpert, &pos, radius, eMoveType, bAvoidObstacles, bIgnorePathPref);
                    LogToFile("    [MoveToPosition] ...MoveToPosition command issued successfully.");

                    // *** CRITICAL: Wait for the action to complete ***
                    if (pVTable->IsPerformingScriptTask) {
                        LogToFile("    [MoveToPosition] STEP 5: Waiting for movement to complete...");
                        int waitFrames = 0;
                        while (pVTable->IsPerformingScriptTask(pExpert)) {
                            NewScriptFrame_API(m_pGameInterface);
                            waitFrames++;

                            // Check for thread termination
                            if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
                                LogToFile("    [MoveToPosition] ...Thread terminated during movement (waited " + std::to_string(waitFrames) + " frames).");
                                break;
                            }
                        }
                        LogToFile("    [MoveToPosition] ...Movement completed after " + std::to_string(waitFrames) + " frames.");
                    }
                    else {
                        LogToFile("!!! WARNING: IsPerformingScriptTask not available - cannot wait for completion!");
                    }
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during MoveToPosition call or wait loop!");
                }
            }
            else {
                LogToFile("!!! ERROR: MoveToPosition function pointer is null in derived VTable!");
            }
        }
        else {
            LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
        }

        // --- Cleanup ---
        LogToFile("    [MoveToPosition] STEP 6: Releasing script control handle...");
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [MoveToPosition] ...Script control handle destroyed.");
    }
    else {
        LogToFile("!!! WARN: Control not acquired, calling destructor on unlinked handle...");
        CSGROSTB_Destroy_API(&seh_me);
    }

    LogToFile("===== MoveToPosition Wrapper END =====");
}

void LuaEntityAPI::MoveToThing(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, float radius, int moveType /*EScriptEntityMoveType*/) {
    LogToFile("===== MoveToThing Wrapper (With Wait Loop) START =====");
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    {
        LogToFile("!!! ERROR: MoveToThing - one or more required game API functions are null!");
        LogToFile("===== MoveToThing Wrapper END (Failure: API Missing) =====");
        return;
    }

    CScriptThing* pTargetThing = spTargetThing.get();

    // --- Input Validation ---
    if (!pMe) {
        LogToFile("!!! ERROR: MoveToThing - pMe is NULL!");
        return;
    }
    if (!pTargetThing) {
        LogToFile("!!! ERROR: MoveToThing - pTargetThing is NULL!");
        return;
    }
    if (!pMe->pImp.Data) {
        LogToFile("!!! ERROR: MoveToThing - pMe->pImp.Data is NULL!");
        return;
    }

    // --- Acquire Script Control ---
    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;

    LogToFile("    [MoveToThing] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr;
    seh_me.pImp.Info = nullptr;

    LogToFile("    [MoveToThing] STEP 2: Attempting to acquire script control...");
    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            LogToFile("    [MoveToThing] ...Control acquired successfully.");
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [MoveToThing] ...Thread is terminating during control acquisition. Aborting.");
            break;
        }
    }

    // --- If Control Acquired, Issue Command and Wait ---
    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
            reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // --- Clear Existing Commands ---
            if (pVTable->ClearCommands) {
                LogToFile("    [MoveToThing] STEP 3: Clearing existing commands...");
                try {
                    pVTable->ClearCommands(pExpert);
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during ClearCommands call!");
                }
            }

            // --- Issue MoveToThing Command ---
            if (pVTable->MoveToThing) {
                LogToFile("    [MoveToThing] STEP 4: Issuing MoveToThing command...");
                EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
                const CScriptThing* pConstTargetThing = pTargetThing;
                float proximity = radius;
                CTCScriptedControl* pWait = nullptr;
                bool bAvoidObstacles = true;
                bool bIgnorePathPref = false;
                bool bFaceMovement = true;

                try {
                    pVTable->MoveToThing(pExpert, pConstTargetThing, proximity, eMoveType,
                        pWait, bAvoidObstacles, bIgnorePathPref, bFaceMovement);
                    LogToFile("    [MoveToThing] ...MoveToThing command issued successfully.");

                    // *** CRITICAL: Wait for the action to complete ***
                    if (pVTable->IsPerformingScriptTask) {
                        LogToFile("    [MoveToThing] STEP 5: Waiting for movement to complete...");
                        int waitFrames = 0;
                        while (pVTable->IsPerformingScriptTask(pExpert)) {
                            NewScriptFrame_API(m_pGameInterface);
                            waitFrames++;

                            // Check for thread termination
                            if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
                                LogToFile("    [MoveToThing] ...Thread terminated during movement (waited " + std::to_string(waitFrames) + " frames).");
                                break;
                            }
                        }
                        LogToFile("    [MoveToThing] ...Movement completed after " + std::to_string(waitFrames) + " frames.");
                    }
                    else {
                        LogToFile("!!! WARNING: IsPerformingScriptTask not available - cannot wait for completion!");
                    }
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during MoveToThing call or wait loop!");
                }
            }
            else {
                LogToFile("!!! ERROR: MoveToThing function pointer is null in derived VTable!");
            }
        }
        else {
            LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
        }

        // --- Cleanup ---
        LogToFile("    [MoveToThing] STEP 6: Releasing script control handle...");
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [MoveToThing] ...Script control handle destroyed.");
    }
    else {
        LogToFile("!!! WARN: Control not acquired, calling destructor on unlinked handle...");
        CSGROSTB_Destroy_API(&seh_me);
    }

    LogToFile("===== MoveToThing Wrapper END =====");
}

void LuaEntityAPI::ClearCommands(CScriptThing* pMe) {
    // B1 fix: this used to call pVTable->ClearCommands(pMe->pImp.Data) on the RAW,
    // un-acquired entity impl. Expert scripted-thing commands require script control
    // first — without it the engine dereferences an unlinked handle (crash/UB). Acquire
    // a transient control handle (blocking-family idiom, cf. PlayAnimation / MoveToThing),
    // issue on the acquired Expert, then release. Instantaneous command: no wait loop.
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API || !g_pCScriptGameResourceObjectScriptedThingBaseVTable) {
        LogToFile("!!! ERROR: ClearCommands - a required game API/VTable pointer is null!");
        return;
    }
    if (!pMe || !pMe->pImp.Data) {
        LogToFile("!!! ERROR: ClearCommands - pMe or pMe->pImp.Data is NULL.");
        return;
    }

    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
    }

    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;
        if (pExpert && pVTable && pVTable->ClearCommands) {
            try { pVTable->ClearCommands(pExpert); }
            catch (...) { LogToFile("!!! EXCEPTION during ClearCommands call!"); }
        }
        else { LogToFile("!!! ERROR: ClearCommands - invalid Expert or null vtable slot."); }
    }
    CSGROSTB_Destroy_API(&seh_me);
}

void LuaEntityAPI::PerformExpression(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, const std::string& expressionName) {
    CScriptGameResourceObjectScriptedThingVTable* pVTable = GetExpertVTable(pMe);
    CScriptThing* pTargetThing = spTargetThing.get(); // Target can be null/optional in game
    if (!pVTable || !pVTable->PerformExpression) return;

    FableString fsExpr(expressionName.c_str());
    pVTable->PerformExpression(pMe->pImp.Data, pTargetThing, fsExpr);
}

void LuaEntityAPI::PlayAnimation(CScriptThing* pMe, const std::string& animName,
    sol::optional<bool> waitForFinish, sol::optional<bool> stayOnLastFrame, sol::optional<bool> allowLooking)
{
    LogToFile("===== PlayAnimation Wrapper (Correct Destructor Cleanup) START ====="); // Log updated
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    // Make sure we have pointers to necessary game functions and the correct destructor
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API || // Use the correct destructor check
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    {
        LogToFile("!!! ERROR: PlayAnimation - one or more required game API functions, VTables, or the CSGROSTB_Destroy_API pointer are null!");
        LogToFile("===== PlayAnimation Wrapper END (Failure: API Missing) =====");
        return;
    }

    // --- Input Validation ---
    if (!pMe) {
        LogToFile("!!! ERROR: PlayAnimation - pMe is NULL.");
        LogToFile("===== PlayAnimation Wrapper END (Failure: pMe Null) =====");
        return;
    }
    if (!pMe->pImp.Data) {
        ss_log << "!!! ERROR: PlayAnimation - pMe->pImp.Data is NULL for pMe 0x" << std::hex << pMe << ".";
        LogToFile(ss_log.str());
        LogToFile("===== PlayAnimation Wrapper END (Failure: pMe->pImp Null) =====");
        return;
    }

    // --- Acquire Script Control ---
    CScriptGameResourceObjectScriptedThingBase seh_me; // Stack-allocated handle
    bool controlAcquired = false;

    LogToFile("    [PlayAnimation] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me); // Construct base object part on stack
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable; // Assign base VTable
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;
    // No Helper1/Helper2 needed for initialization here based on assembly

    LogToFile("    [PlayAnimation] STEP 2: Attempting to acquire script control...");
    while (true) { // Loop to acquire control
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            LogToFile("    [PlayAnimation] ...Control acquired successfully.");
            break; // Exit loop on success
        }

        // Yield if control failed
        NewScriptFrame_API(m_pGameInterface);

        // Check for termination
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [PlayAnimation] ...Thread is terminating during control acquisition. Aborting.");
            break; // Exit loop if script is ending
        }
    }

    // --- If Control Acquired, Issue Command ---
    if (controlAcquired) {
        // Get the actual implementation pointer (Expert) and its VTable from the handle
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // --- Clear Existing Commands ---
            if (pVTable->ClearCommands) {
                LogToFile("    [PlayAnimation] STEP 3: Clearing existing commands...");
                try { pVTable->ClearCommands(pExpert); }
                catch (...) { LogToFile("!!! EXCEPTION during ClearCommands call!"); }
            }
            else { LogToFile("!!! WARNING: ClearCommands function pointer is null in derived VTable!"); }

            // --- Prepare Parameters and Call PlayAnimation ---
            if (pVTable->PlayAnimation) {
                LogToFile("    [PlayAnimation] STEP 4: Issuing PlayAnimation command...");
                FableString fsAnim(animName.c_str());

                // Determine blocking flag from Lua input
                bool bWaitForFinish = waitForFinish.value_or(true); // Default to blocking if Lua provides nil or nothing

                // Set other flags based on Lua input or defaults
                bool bStayOnLastFrame = stayOnLastFrame.value_or(false);
                bool bAllowLooking = allowLooking.value_or(false);
                bool overwrite_actions = false; // We clear manually
                bool add_as_queued = false;
                bool use_physics = true;
                bool camera_update = false;

                // Log parameters before call
                ss_log.str("");
                ss_log << "  Parameters for API call:"
                    << "\n    this (pExpert): 0x" << std::hex << pExpert
                    << "\n    animName: " << animName
                    << "\n    stay_on_last_frame: " << (bStayOnLastFrame ? "true" : "false")
                    << "\n    overwrite_existing_actions: 0x" << std::hex << overwrite_actions
                    << "\n    add_as_queued_action: " << (add_as_queued ? "true" : "false")
                    << "\n    wait_for_anim_to_finish: " << (bWaitForFinish ? "true" : "false") // Passed to game
                    << "\n    use_physics: " << (use_physics ? "true" : "false")
                    << "\n    camera_update: " << (camera_update ? "true" : "false")
                    << "\n    allow_looking: " << (bAllowLooking ? "true" : "false");
                LogToFile(ss_log.str());

                try {
                    // Call the VTable function using pExpert as 'this'
                    pVTable->PlayAnimation(
                        pExpert,
                        fsAnim,
                        bStayOnLastFrame,
                        overwrite_actions,
                        add_as_queued,
                        bWaitForFinish,      // Pass blocking flag to game
                        use_physics,
                        camera_update,
                        bAllowLooking
                    );
                    LogToFile("    [PlayAnimation] ...PlayAnimation command issued successfully.");

                    // --- Conditional Wait Loop (If blocking was requested) ---
                    if (bWaitForFinish) {
                        LogToFile("    [PlayAnimation] STEP 4.5: Entering wait loop (blocking)...");
                        if (pVTable->IsPerformingScriptTask) {
                            while (pVTable->IsPerformingScriptTask(pExpert)) {
                                NewScriptFrame_API(m_pGameInterface); // Yield
                                if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
                                    LogToFile("    [PlayAnimation] ...Thread terminated during wait loop.");
                                    break; // Exit wait loop if script is ending
                                }
                            }
                            LogToFile("    [PlayAnimation] ...Wait loop finished.");
                        }
                        else {
                            LogToFile("!!! WARNING: IsPerformingScriptTask function pointer is null! Cannot block properly.");
                        }
                    } // --- End Wait Loop ---

                }
                catch (const std::exception& e) {
                    ss_log.str("");
                    ss_log << "!!! C++ EXCEPTION during PlayAnimation call or wait loop: " << e.what();
                    LogToFile(ss_log.str());
                }
                catch (...) {
                    LogToFile("!!! UNKNOWN C++ EXCEPTION during PlayAnimation call or wait loop!");
                }

            }
            else { LogToFile("!!! ERROR: PlayAnimation function pointer is null in derived VTable!"); }
        }
        else { LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!"); }

        // --- CORRECTED ASSEMBLY-ACCURATE Cleanup ---
        LogToFile("    [PlayAnimation] STEP 5: Releasing script control handle (seh_me) via Correct Destructor ONLY...");
        // Call the specific destructor for CScriptGameResourceObjectScriptedThingBase
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [PlayAnimation] ...Script control handle object destroyed via CSGROSTB_Destroy_API.");
        // No Helper2 needed here as the correct destructor should handle full release.
        // --- End Corrected Cleanup ---

    }
    else {
        // Control acquisition failed cleanup
        LogToFile("!!! WARN: Control not acquired, calling CSGROSTB_Destroy_API on potentially unlinked handle...");
        CSGROSTB_Destroy_API(&seh_me); // Still need to call destructor on the stack object
    }
    LogToFile("===== PlayAnimation Wrapper END =====");
}

void LuaEntityAPI::PlayCombatAnimation(CScriptThing* pMe, const std::string& animName,
    sol::optional<bool> waitForFinish, sol::optional<bool> allowLooking) // Keep optionals for flexibility, override below
{
    LogToFile("===== PlayCombatAnimation Wrapper (Correct Cleanup) START =====");
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    { /* ... Error Log ... */ return;
    }

    // --- Input Validation ---
    if (!pMe) { /* ... Null check log ... */ return; }
    if (!pMe->pImp.Data) { /* ... Null check log ... */ return; }

    // --- Acquire Script Control ---
    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;

    LogToFile("    [PlayCombatAnimation] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    // seh_me.pVTable = ...; // No need to set VTable manually
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    LogToFile("    [PlayCombatAnimation] STEP 2: Attempting to acquire script control...");
    while (true) { // Loop to acquire control
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true; LogToFile("    [PlayCombatAnimation] ...Control acquired successfully."); break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [PlayCombatAnimation] ...Thread is terminating during control acquisition. Aborting."); break;
        }
    }

    // --- If Control Acquired, Issue Command ---
    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // --- Clear Existing Commands ---
            if (pVTable->ClearCommands) { /* ... ClearCommands call ... */ }
            else { /* ... Warning log ... */ }

            // --- Prepare Parameters and Call PlayCombatAnimation ---
            if (pVTable->PlayCombatAnimation) { // Check function pointer
                LogToFile("    [PlayCombatAnimation] STEP 4: Issuing PlayCombatAnimation command...");
                FableString fsAnim(animName.c_str());

                // --- Set parameters based on user request ---
                bool bWaitForFinish = true; // Hardcoded true as requested
                bool bAllowLooking = true;  // Hardcoded true as requested
                bool use_physics = true;    // Hardcoded true as requested
                bool overwrite_actions = false;
                bool add_as_queued = true;   // Hardcoded true as requested
                bool camera_update = true;   // Hardcoded true as requested
                // --- End Parameter Setup ---

                ss_log.str(""); ss_log << "  Parameters for API call:" /* ... logging ... */; LogToFile(ss_log.str());

                try {
                    // Call the VTable function using pExpert as 'this'
                    pVTable->PlayCombatAnimation(
                        pExpert,
                        fsAnim,
                        use_physics,
                        overwrite_actions,
                        add_as_queued,
                        bWaitForFinish,      // Pass blocking flag to game
                        camera_update,
                        bAllowLooking
                    );
                    LogToFile("    [PlayCombatAnimation] ...PlayCombatAnimation command issued successfully.");

                    // --- Conditional Wait Loop (If blocking was requested) ---
                    if (bWaitForFinish) { // Always true based on hardcoded value above
                        LogToFile("    [PlayCombatAnimation] STEP 4.5: Entering wait loop (blocking)...");
                        if (pVTable->IsPerformingScriptTask) {
                            while (pVTable->IsPerformingScriptTask(pExpert)) {
                                NewScriptFrame_API(m_pGameInterface); // Yield
                                if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
                            }
                            LogToFile("    [PlayCombatAnimation] ...Wait loop finished.");
                        }
                        else { /* ... Warning log ... */ }
                    } // --- End Wait Loop ---

                }
                catch (...) { /* Exception log */ }

            }
            else { LogToFile("!!! ERROR: PlayCombatAnimation function pointer is null in derived VTable!"); }
        }
        else { /* Error log */ }

        // --- CORRECTED ASSEMBLY-ACCURATE Cleanup ---
        LogToFile("    [PlayCombatAnimation] STEP 5: Releasing script control handle (seh_me) via Correct Destructor ONLY...");
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [PlayCombatAnimation] ...Script control handle object destroyed via CSGROSTB_Destroy_API.");
        // --- End Corrected Cleanup ---

    }
    else { /* ... Failed acquisition cleanup ... */ }

    LogToFile("===== PlayCombatAnimation Wrapper END =====");
}

void LuaEntityAPI::PlayLoopingAnimation(CScriptThing* pMe, const std::string& animName, int loopCount,
    sol::optional<bool> waitForFinish, sol::optional<bool> useMovement, sol::optional<bool> allowLooking) // Keep allowLooking optional
{
    LogToFile("===== PlayLoopingAnimation Wrapper (Correct Cleanup) START =====");
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    { /* ... Error Log ... */ return;
    }

    // --- Input Validation ---
    if (!pMe) { /* ... Null check log ... */ return; }
    if (!pMe->pImp.Data) { /* ... Null check log ... */ return; }

    // --- Acquire Script Control ---
    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;

    LogToFile("    [PlayLoopingAnimation] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    // seh_me.pVTable = ...; // No need to set VTable manually
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    LogToFile("    [PlayLoopingAnimation] STEP 2: Attempting to acquire script control...");
    while (true) { // Loop to acquire control
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true; LogToFile("    [PlayLoopingAnimation] ...Control acquired successfully."); break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [PlayLoopingAnimation] ...Thread is terminating during control acquisition. Aborting."); break;
        }
    }

    // --- If Control Acquired, Issue Command ---
    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // --- Clear Existing Commands ---
            if (pVTable->ClearCommands) { /* ... ClearCommands call ... */ }
            else { /* ... Warning log ... */ }

            // --- Prepare Parameters and Call PlayLoopingAnimation ---
            if (pVTable->PlayLoopingAnimation) { // Check function pointer
                LogToFile("    [PlayLoopingAnimation] STEP 4: Issuing PlayLoopingAnimation command...");
                FableString fsAnim(animName.c_str());

                // --- Set parameters based on user request ---
                bool bWaitForFinish = waitForFinish.value_or(false); // Default non-blocking for loops
                bool bUseMovement = useMovement.value_or(false);     // Default no root motion for loops
                bool bAllowLooking = true; // Hardcoded true as requested
                int num_loops = loopCount; // Direct from Lua

                // Hardcoded defaults based on user request / common sense
                bool overwrite_actions = false;
                bool add_as_queued = true;   // Hardcoded true as requested
                bool use_physics = true;     // Hardcoded true as requested
                bool camera_update = true;   // Hardcoded true as requested
                // --- End Parameter Setup ---


                ss_log.str(""); ss_log << "  Parameters for API call:" /* ... logging ... */; LogToFile(ss_log.str());

                try {
                    // Call the VTable function using pExpert as 'this'
                    pVTable->PlayLoopingAnimation(
                        pExpert,
                        fsAnim,
                        num_loops,
                        bUseMovement,
                        overwrite_actions,
                        add_as_queued,
                        bWaitForFinish,      // Pass blocking flag to game
                        use_physics,
                        camera_update,
                        bAllowLooking
                    );
                    LogToFile("    [PlayLoopingAnimation] ...PlayLoopingAnimation command issued successfully.");

                    // --- Conditional Wait Loop (If blocking was requested) ---
                    if (bWaitForFinish) {
                        LogToFile("    [PlayLoopingAnimation] STEP 4.5: Entering wait loop (blocking)...");
                        if (pVTable->IsPerformingScriptTask) {
                            while (pVTable->IsPerformingScriptTask(pExpert)) {
                                NewScriptFrame_API(m_pGameInterface); // Yield
                                if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
                            }
                            LogToFile("    [PlayLoopingAnimation] ...Wait loop finished.");
                        }
                        else { /* ... Warning log ... */ }
                    } // --- End Wait Loop ---

                }
                catch (...) { /* Exception log */ }

            }
            else { LogToFile("!!! ERROR: PlayLoopingAnimation function pointer is null in derived VTable!"); }
        }
        else { /* Error log */ }

        // --- CORRECTED ASSEMBLY-ACCURATE Cleanup ---
        LogToFile("    [PlayLoopingAnimation] STEP 5: Releasing script control handle (seh_me) via Correct Destructor ONLY...");
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [PlayLoopingAnimation] ...Script control handle object destroyed via CSGROSTB_Destroy_API.");
        // --- End Corrected Cleanup ---

    }
    else { /* ... Failed acquisition cleanup ... */ }

    LogToFile("===== PlayLoopingAnimation Wrapper END =====");
}

void LuaEntityAPI::MoveToAndPickUpGenericBox(CScriptThing* pMe, CScriptThing* pBox, int moveType, sol::optional<bool> avoidObstacles) {
    // TODO(B1, DEFERRED): still calls the expert vtable on the raw un-acquired impl.
    // Unlike the instantaneous commands (ClearCommands/DropGenericBox/UnsheatheWeapons),
    // this is a BLOCKING move — fixing it needs the acquire + IsPerformingScriptTask wait
    // loop (cf. MoveToThing) and live in-game verification. Do not "fix" by issue-and-release:
    // releasing control immediately would cancel the queued move. Left as-is pending a test pass.
    CScriptGameResourceObjectScriptedThingVTable* pVTable = GetExpertVTable(pMe);
    // Note: This API might be on the Base VTable (CScriptGameResourceObjectScriptedThingBaseVTable)
    // Adjust GetExpertVTable or use the correct VTable pointer if necessary.
    // Assuming GetExpertVTable works for now.
    if (!pVTable || !pVTable->MoveToAndPickUpGenericBox || !pBox) return;

    EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
    BOOL bAvoidObstacles = avoidObstacles.value_or(true); // Default: try to avoid

    // Call the function using pMe->pImp.Data as 'This'
    pVTable->MoveToAndPickUpGenericBox(pMe->pImp.Data, pBox, eMoveType, bAvoidObstacles);
}

void LuaEntityAPI::DropGenericBox(CScriptThing* pMe) {
    // B1 fix: acquire transient script control before issuing the expert command
    // (previously called on the raw un-acquired impl). See ClearCommands above.
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API || !g_pCScriptGameResourceObjectScriptedThingBaseVTable) {
        LogToFile("!!! ERROR: DropGenericBox - a required game API/VTable pointer is null!");
        return;
    }
    if (!pMe || !pMe->pImp.Data) {
        LogToFile("!!! ERROR: DropGenericBox - pMe or pMe->pImp.Data is NULL.");
        return;
    }

    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
    }

    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;
        if (pExpert && pVTable && pVTable->DropGenericBox) {
            try { pVTable->DropGenericBox(pExpert); }
            catch (...) { LogToFile("!!! EXCEPTION during DropGenericBox call!"); }
        }
        else { LogToFile("!!! ERROR: DropGenericBox - invalid Expert or null vtable slot."); }
    }
    CSGROSTB_Destroy_API(&seh_me);
}

void LuaEntityAPI::UnsheatheWeapons(CScriptThing* pMe) {
    // B1 fix: acquire transient script control before issuing the expert command
    // (previously called on the raw un-acquired impl). See ClearCommands above.
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API || !g_pCScriptGameResourceObjectScriptedThingBaseVTable) {
        LogToFile("!!! ERROR: UnsheatheWeapons - a required game API/VTable pointer is null!");
        return;
    }
    if (!pMe || !pMe->pImp.Data) {
        LogToFile("!!! ERROR: UnsheatheWeapons - pMe or pMe->pImp.Data is NULL.");
        return;
    }

    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) break;
    }

    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;
        if (pExpert && pVTable && pVTable->UnsheatheWeapons) {
            try { pVTable->UnsheatheWeapons(pExpert); }
            catch (...) { LogToFile("!!! EXCEPTION during UnsheatheWeapons call!"); }
        }
        else { LogToFile("!!! ERROR: UnsheatheWeapons - invalid Expert or null vtable slot."); }
    }
    CSGROSTB_Destroy_API(&seh_me);
}

void LuaEntityAPI::Wait(CScriptThing* pMe, float seconds) {
    // TODO(B1, DEFERRED): still calls the expert vtable on the raw un-acquired impl.
    // This is a BLOCKING wait — a naive acquire + issue-and-release would cancel the
    // queued wait, making it a no-op. Needs acquire + a proper yield/wait loop and live
    // in-game verification before converting. Left as-is pending a test pass.
    CScriptGameResourceObjectScriptedThingVTable* pVTable = GetExpertVTable(pMe);
    if (!pVTable || !pVTable->Wait) return;

    pVTable->Wait(pMe->pImp.Data, seconds);
}

bool LuaEntityAPI::IsAlive(CScriptThing* pMe) {
    if (!pMe) return false;
    // Get implementation pointer and vtable
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    // Check function pointer validity
    if (!pVTable->IsAlive) {
        // LogToFile("!!! ERROR: IsAlive - function pointer is null in VTable!"); // Optional log
        return false; // Assume not alive if check fails
    }
    // Call the function
    return pVTable->IsAlive(pImp);
}

bool LuaEntityAPI::IsDead(CScriptThing* pMe) {
    if (!pMe) return true; // Treat null pointer as "dead" or invalid
    // Get implementation pointer and vtable
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return true; // Treat invalid implementation as "dead"
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    // Check function pointer validity
    if (!pVTable->IsDead) {
        // LogToFile("!!! ERROR: IsDead - function pointer is null in VTable!"); // Optional log
        return true; // Assume dead if check fails
    }
    // Call the function
    return pVTable->IsDead(pImp);
}

void LuaEntityAPI::TakeExclusiveControl(CScriptThing* pMe)
{
    if (!m_pGameInterface || !pMe || !EntitySetAllStategroupsEnabled_API || !EntitySetOpinionReactionsEnabled_API || !EntitySetDeedReactionsEnabled_API)
    {
        return;
    }
    EntitySetAllStategroupsEnabled_API(m_pGameInterface, pMe, false);
    EntitySetOpinionReactionsEnabled_API(m_pGameInterface, pMe, true);
    EntitySetDeedReactionsEnabled_API(m_pGameInterface, pMe, true);
}

void LuaEntityAPI::FollowThing(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, float distance, bool avoidObstacles) {
    LogToFile("===== FollowThing Wrapper START =====");
    std::stringstream ss_log;

    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    {
        LogToFile("!!! ERROR: FollowThing - one or more required game API functions are null!");
        LogToFile("===== FollowThing Wrapper END (Failure: API Missing) =====");
        return;
    }

    CScriptThing* pTargetThing = spTargetThing.get();

    if (!pMe) {
        LogToFile("!!! ERROR: FollowThing - pMe is NULL!");
        return;
    }
    if (!pTargetThing) {
        LogToFile("!!! ERROR: FollowThing - pTargetThing is NULL!");
        return;
    }
    if (!pMe->pImp.Data) {
        LogToFile("!!! ERROR: FollowThing - pMe->pImp.Data is NULL!");
        return;
    }

    ss_log << "    Target: 0x" << std::hex << pTargetThing
        << ", distance: " << std::dec << distance
        << ", avoidObstacles: " << (avoidObstacles ? "true" : "false");
    LogToFile(ss_log.str());

    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool controlAcquired = false;

    LogToFile("    [FollowThing] STEP 1: Preparing script control handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    seh_me.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    seh_me.pImp.Data = nullptr;
    seh_me.pImp.Info = nullptr;

    LogToFile("    [FollowThing] STEP 2: Attempting to acquire script control...");
    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true;
            LogToFile("    [FollowThing] ...Control acquired successfully.");
            break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [FollowThing] ...Thread is terminating during control acquisition. Aborting.");
            break;
        }
    }

    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
            reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            if (pVTable->ClearCommands) {
                LogToFile("    [FollowThing] STEP 3: Clearing existing commands...");
                try {
                    pVTable->ClearCommands(pExpert);
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during ClearCommands call!");
                }
            }

            if (pVTable->FollowThing) {
                LogToFile("    [FollowThing] STEP 4: Issuing FollowThing command...");
                const CScriptThing* pConstTargetThing = pTargetThing;

                ss_log.str("");
                ss_log << "  Parameters for API call:"
                    << "\n    this (pExpert): 0x" << std::hex << pExpert
                    << "\n    target: 0x" << std::hex << pConstTargetThing
                    << "\n    follow_range: " << std::dec << distance
                    << "\n    avoid_dynamic_obstacles: " << (avoidObstacles ? "true" : "false");
                LogToFile(ss_log.str());

                try {
                    pVTable->FollowThing(pExpert, pConstTargetThing, distance, avoidObstacles);
                    LogToFile("    [FollowThing] ...FollowThing command issued successfully (action queued).");

                    // NOTE: Unlike MoveToThing, FollowThing doesn't wait - it's a continuous action
                    // that runs until explicitly stopped. We release control immediately.
                }
                catch (...) {
                    LogToFile("!!! EXCEPTION during FollowThing call!");
                }
            }
            else {
                LogToFile("!!! ERROR: FollowThing function pointer is null in derived VTable!");
            }
        }
        else {
            LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
        }

        LogToFile("    [FollowThing] STEP 5: Releasing script control handle...");
        CSGROSTB_Destroy_API(&seh_me);
        LogToFile("    [FollowThing] ...Script control handle destroyed.");
    }
    else {
        LogToFile("!!! WARN: Control not acquired, calling destructor on unlinked handle...");
        CSGROSTB_Destroy_API(&seh_me);
    }

    LogToFile("===== FollowThing Wrapper END =====");
}

void LuaEntityAPI::StopFollowingThing(CScriptThing* pMe, CScriptThing* pTargetThing) {
    LogToFile("===== StopFollowingThing Wrapper (Correct Cleanup) START =====");
    std::stringstream ss_log;

    // --- Prerequisite Checks ---
    if (!m_pGameInterface || !StartScriptingEntity_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API ||
        !CBaseObject_Construct_API || !CSGROSTB_Destroy_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable)
    { /* Log Error */ return;
    }

    // --- Input Validation ---
    if (!pMe) { LogToFile("!!! ERROR: StopFollowingThing - pMe is NULL."); /* ... */ return; }
    if (!pTargetThing) { LogToFile("!!! ERROR: StopFollowingThing - pTargetThing is NULL."); /* ... */ return; } // Assuming target is required
    if (!pMe->pImp.Data) { LogToFile("!!! ERROR: StopFollowingThing - pMe->pImp.Data is NULL."); /* ... */ return; }

    // --- Initialize Handle FIRST ---
    CScriptGameResourceObjectScriptedThingBase seh_me;
    LogToFile("    [StopFollowingThing] STEP 1a: Constructing base handle (seh_me)...");
    CBaseObject_Construct_API(&seh_me);
    seh_me.pImp.Data = nullptr; seh_me.pImp.Info = nullptr;

    // --- Acquire Script Control ---
    bool controlAcquired = false;
    LogToFile("    [StopFollowingThing] STEP 1b: Attempting to acquire script control...");
    while (true) {
        if (StartScriptingEntity_API(m_pGameInterface, pMe, &seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            controlAcquired = true; LogToFile("    [StopFollowingThing] ...Control acquired."); break;
        }
        NewScriptFrame_API(m_pGameInterface);
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            LogToFile("    [StopFollowingThing] ...Thread terminating during acquire."); goto cleanup; // Use goto
        }
    }

    // --- Issue Command ---
    if (controlAcquired) {
        CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(seh_me.pImp.Data);
        CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

        if (pExpert && pVTable) {
            // Optional: Clear Commands first? Might not be needed for Stop.
            // if (pVTable->ClearCommands) { LogToFile("    [StopFollowingThing] STEP 3: Clearing commands..."); pVTable->ClearCommands(pExpert); }

            // Call StopFollowingThing
            if (pVTable->StopFollowingThing) {
                LogToFile("    [StopFollowingThing] STEP 4: Issuing StopFollowingThing command...");
                const CScriptThing* pConstTargetThing = pTargetThing; // API expects const? Verify signature.

                ss_log.str("");
                ss_log << "  Parameters for API call:"
                    << "\n    this (pExpert): 0x" << std::hex << pExpert
                    << "\n    target: 0x" << std::hex << pConstTargetThing;
                LogToFile(ss_log.str());

                try {
                    pVTable->StopFollowingThing(pExpert, pConstTargetThing);
                    LogToFile("    [StopFollowingThing] ...Command issued.");
                }
                catch (const std::exception& e) {
                    ss_log.str(""); ss_log << "!!! C++ EXCEPTION during StopFollowingThing call: " << e.what(); LogToFile(ss_log.str());
                }
                catch (...) { LogToFile("!!! UNKNOWN C++ EXCEPTION during StopFollowingThing call!"); }
            }
            else { LogToFile("!!! ERROR: StopFollowingThing function pointer is null in derived VTable!"); }
        }
        else { LogToFile("!!! ERROR: Invalid pExpert or pVTable after acquiring control!"); }
    }
    else { LogToFile("!!! WARN: Control not acquired after loop exit."); }

cleanup:
    LogToFile("    [StopFollowingThing] STEP Cleanup: Releasing handle...");
    CSGROSTB_Destroy_API(&seh_me); // Use correct destructor
    LogToFile("    [StopFollowingThing] ...Handle destroyed.");

    LogToFile("===== StopFollowingThing Wrapper END =====");
}

bool LuaEntityAPI::IsFollowActionRunning(CScriptThing* pMe, CScriptThing* pTargetThing) {
    // LogToFile("===== IsFollowActionRunning Wrapper START =====");
    if (!pMe || !pTargetThing || !pMe->pImp.Data) { /* Log Error */ return false; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(pMe->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable && pVTable->IsFollowActionRunning) {
        try {
            const CScriptThing* pConstTargetThing = pTargetThing; // API expects const?
            return pVTable->IsFollowActionRunning(pExpert, pConstTargetThing);
        }
        catch (...) { /* Log Exception */ return false; }
    }
    else { /* Log Error: Invalid pointers or function pointer null */ return false; }
    // LogToFile("===== IsFollowActionRunning Wrapper END =====");
}

bool LuaEntityAPI::IsFollowingThing(CScriptThing* pMe) {
    // LogToFile("===== IsFollowingThing Wrapper START =====");
    if (!pMe || !pMe->pImp.Data) { /* Log Error */ return false; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(pMe->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable && pVTable->IsFollowingThing) {
        try {
            return pVTable->IsFollowingThing(pExpert);
        }
        catch (...) { /* Log Exception */ return false; }
    }
    // ELSE: Check Base VTable if necessary, as discussed before.
    else { /* Log Error: Invalid pointers or function pointer null */ return false; }
    // LogToFile("===== IsFollowingThing Wrapper END =====");
}

void LuaEntityAPI::MoveToThing_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing,
    float radius, int moveType, sol::optional<bool> avoidObstacles,
    sol::optional<bool> ignorePathPreferability, sol::optional<bool> faceMovement) {
    SelectControlHandle(pMe);
    LogToFile("===== MoveToThing_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: MoveToThing_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== MoveToThing_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    CScriptThing* pTargetThing = spTargetThing.get();

    // 2. Input Validation
    if (!pTargetThing) {
        LogToFile("!!! ERROR: MoveToThing_NonBlocking - pTargetThing is NULL!");
        LogToFile("===== MoveToThing_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 3. Get the Expert object and VTable from the *existing* handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue MoveToThing Command ---
        if (pVTable->MoveToThing) {
            LogToFile("    [MoveToThing_NonBlocking] Issuing MoveToThing command...");
            EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
            const CScriptThing* pConstTargetThing = pTargetThing;
            float proximity = radius;
            CTCScriptedControl* pWait = nullptr;
            bool bAvoidObstacles = avoidObstacles.value_or(true);
            bool bIgnorePathPref = ignorePathPreferability.value_or(false);
            bool bFaceMovement = faceMovement.value_or(true);

            try {
                pVTable->MoveToThing(pExpert, pConstTargetThing, proximity, eMoveType,
                    pWait, bAvoidObstacles, bIgnorePathPref, bFaceMovement);
                LogToFile("    [MoveToThing_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during MoveToThing_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: MoveToThing function pointer is null in derived VTable!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
    }

    // 4. RETURN IMMEDIATELY
    LogToFile("===== MoveToThing_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::PlayAnimation_NonBlocking(CScriptThing* pMe, const std::string& animName,
    sol::optional<bool> b1, sol::optional<bool> b2, sol::optional<bool> b3, sol::optional<bool> b4,
    sol::optional<bool> b5, sol::optional<bool> b6, sol::optional<bool> b7)
{
    SelectControlHandle(pMe);
    LogToFile("===== PlayAnimation_NonBlocking Wrapper START =====");
    std::stringstream ss_log;

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: PlayAnimation_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== PlayAnimation_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Get the Expert object and VTable from the *existing* handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue PlayAnimation Command ---
        if (pVTable->PlayAnimation) {
            LogToFile("    [PlayAnimation_NonBlocking] Issuing PlayAnimation command for '" + animName + "'...");
            FableString fsAnim(animName.c_str());

            try {
                pVTable->PlayAnimation(
                    pExpert, fsAnim,
                    b1.value_or(false), b2.value_or(false), b3.value_or(false),
                    b4.value_or(false), b5.value_or(true), b6.value_or(false),
                    b7.value_or(false)
                );
                LogToFile("    [PlayAnimation_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during PlayAnimation_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: PlayAnimation function pointer is null in derived VTable!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
    }

    // 4. RETURN IMMEDIATELY
    LogToFile("===== PlayAnimation_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::PlayLoopingAnimation_NonBlocking(CScriptThing* pMe, const std::string& animName, int loopCount,
    sol::optional<bool> b1, sol::optional<bool> b2, sol::optional<bool> b3, sol::optional<bool> b4,
    sol::optional<bool> b5, sol::optional<bool> b6, sol::optional<bool> b7)
{
    SelectControlHandle(pMe);
    LogToFile("===== PlayLoopingAnimation_NonBlocking Wrapper START =====");
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) { /* Log Error & Return */ return; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        if (pVTable->PlayLoopingAnimation) {
            FableString fsAnim(animName.c_str());
            try {
                pVTable->PlayLoopingAnimation(
                    pExpert, fsAnim, loopCount,
                    b1.value_or(false), b2.value_or(true), b3.value_or(false),
                    b4.value_or(false), b5.value_or(true), b6.value_or(false),
                    b7.value_or(false)
                );
                // Log success
            }
            catch (...) { /* Log Exception */ }
        }
        else { /* Log Error: Function pointer null */ }
    }
    else { /* Log Error: Invalid handle */ }
    LogToFile("===== PlayLoopingAnimation_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::PlayCombatAnimation_NonBlocking(CScriptThing* pMe, const std::string& animName,
    sol::optional<bool> b1, sol::optional<bool> b2, sol::optional<bool> b3,
    sol::optional<bool> b4, sol::optional<bool> b5, sol::optional<bool> b6)
{
    SelectControlHandle(pMe);
    LogToFile("===== PlayCombatAnimation_NonBlocking Wrapper START =====");
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) { /* Log Error & Return */ return; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        if (pVTable->PlayCombatAnimation) {
            FableString fsAnim(animName.c_str());
            try {
                pVTable->PlayCombatAnimation(
                    pExpert, fsAnim,
                    b1.value_or(true), b2.value_or(false), b3.value_or(false),
                    b4.value_or(false), b5.value_or(false), b6.value_or(false)
                );
                // Log success
            }
            catch (...) { /* Log Exception */ }
        }
        else { /* Log Error: Function pointer null */ }
    }
    else { /* Log Error: Invalid handle */ }
    LogToFile("===== PlayCombatAnimation_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::MoveToPosition_NonBlocking(CScriptThing* pMe, sol::table position, float radius, int moveType,
    sol::optional<bool> b1, sol::optional<bool> b2) {
    SelectControlHandle(pMe);
    LogToFile("===== MoveToPosition_NonBlocking Wrapper START =====");
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: MoveToPosition_NonBlocking - No control handle for requested entity.");
        return;
    }

    // Extract position safely
    sol::optional<float> optX = position["x"];
    sol::optional<float> optY = position["y"];
    sol::optional<float> optZ = position["z"];
    C3DVector pos = { optX.value_or(0.0f), optY.value_or(0.0f), optZ.value_or(0.0f) };

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        if (pVTable->MoveToPosition) {
            EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
            bool bAvoidObstacles = b1.value_or(true);
            bool bIgnorePathPref = b2.value_or(false);

            {
                std::ostringstream ss;
                ss << "    [MoveToPositionDiag] thing=0x" << std::hex
                   << reinterpret_cast<uintptr_t>(pMe)
                   << " thingData=0x" << reinterpret_cast<uintptr_t>(pMe ? pMe->pImp.Data : nullptr)
                   << " handle=0x" << reinterpret_cast<uintptr_t>(m_pControlHandle)
                   << " resourceData=0x" << reinterpret_cast<uintptr_t>(m_pControlHandle->pImp.Data)
                   << std::dec << " destination=(" << pos.x << "," << pos.y << "," << pos.z << ")"
                   << " radius=" << radius << " moveType=" << moveType
                   << " avoidObstacles=" << bAvoidObstacles
                   << " ignorePathPreferability=" << bIgnorePathPref;
                LogToFile(ss.str());
            }

            try {
                pVTable->MoveToPosition(pExpert, &pos, radius, eMoveType, bAvoidObstacles, bIgnorePathPref);
                // Log success
            }
            catch (...) { /* Log Exception */ }
        }
        else { /* Log Error: Function pointer null */ }
    }
    else { /* Log Error: Invalid handle */ }
    LogToFile("===== MoveToPosition_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::FollowThing_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, float distance, bool avoidObstacles) {
    SelectControlHandle(pMe);
    LogToFile("===== FollowThing_NonBlocking Wrapper START =====");
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) { /* Log Error & Return */ return; }

    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) { /* Log Error & Return */ return; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        if (pVTable->FollowThing) {
            const CScriptThing* pConstTargetThing = pTargetThing;
            try {
                pVTable->FollowThing(pExpert, pConstTargetThing, distance, avoidObstacles);
                // Log success
            }
            catch (...) { /* Log Exception */ }
        }
        else { /* Log Error: Function pointer null */ }
    }
    else { /* Log Error: Invalid handle */ }
    LogToFile("===== FollowThing_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::ClearCommands_NonBlocking(CScriptThing* pMe) {
    SelectControlHandle(pMe);
    LogToFile("===== ClearCommands_NonBlocking Wrapper START =====");
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) { /* Log Error & Return */ return; }

    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ? reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable && pVTable->ClearCommands) {
        try {
            pVTable->ClearCommands(pExpert);
            // Log success
        }
        catch (...) { /* Log Exception */ }
    }
    else { /* Log Error: Invalid handle or function pointer null */ }
    LogToFile("===== ClearCommands_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::StopFollowingThing_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing) {
    LogToFile("===== StopFollowingThing_NonBlocking Wrapper START =====");

    // 1. Check if we have the main handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: StopFollowingThing_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== StopFollowingThing_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    CScriptThing* pTargetThing = spTargetThing.get();

    // 2. Input Validation
    if (!pTargetThing) {
        LogToFile("!!! ERROR: StopFollowingThing_NonBlocking - pTargetThing is NULL!");
        LogToFile("===== StopFollowingThing_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 3. Get Expert and VTable from our existing handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue StopFollowingThing Command ---
        if (pVTable->StopFollowingThing) {
            LogToFile("    [StopFollowingThing_NonBlocking] Issuing StopFollowingThing command...");
            const CScriptThing* pConstTargetThing = pTargetThing;

            try {
                pVTable->StopFollowingThing(pExpert, pConstTargetThing);
                LogToFile("    [StopFollowingThing_NonBlocking] ...StopFollowingThing command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during StopFollowingThing_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: StopFollowingThing function pointer is null in derived VTable!");
        }

        // --- CRITICAL: Clear all movement commands after stopping follow ---
        if (pVTable->ClearCommands) {
            LogToFile("    [StopFollowingThing_NonBlocking] Clearing movement commands...");
            try {
                pVTable->ClearCommands(pExpert);
                LogToFile("    [StopFollowingThing_NonBlocking] ...Commands cleared successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during ClearCommands in StopFollowingThing_NonBlocking!");
            }
        }
        else {
            LogToFile("!!! WARNING: ClearCommands function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable from handle!");
    }

    // 4. RETURN IMMEDIATELY
    LogToFile("===== StopFollowingThing_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::MoveToAndPickUpGenericBox_NonBlocking(CScriptThing* pMe, CScriptThing* pBox, int moveType, sol::optional<bool> avoidObstacles) {
    LogToFile("===== MoveToAndPickUpGenericBox_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: MoveToAndPickUpGenericBox_NonBlocking - No control handle! Did you call AcquireControl()?");
        return;
    }

    // 2. Input Validation
    if (!pBox) {
        LogToFile("!!! ERROR: MoveToAndPickUpGenericBox_NonBlocking - pBox is NULL!");
        return;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Clear existing commands ---
        if (pVTable->ClearCommands) {
            LogToFile("    [MoveToAndPickUpGenericBox_NonBlocking] Clearing existing commands...");
            pVTable->ClearCommands(pExpert);
        }

        // --- Issue MoveToAndPickUpGenericBox Command ---
        if (pVTable->MoveToAndPickUpGenericBox) {
            LogToFile("    [MoveToAndPickUpGenericBox_NonBlocking] Issuing command...");
            EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
            BOOL bAvoidObstacles = avoidObstacles.value_or(true); // Default: try to avoid

            try {
                pVTable->MoveToAndPickUpGenericBox(pExpert, pBox, eMoveType, bAvoidObstacles);
                LogToFile("    [MoveToAndPickUpGenericBox_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during MoveToAndPickUpGenericBox_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: MoveToAndPickUpGenericBox function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== MoveToAndPickUpGenericBox_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::DropGenericBox_NonBlocking(CScriptThing* pMe) {
    LogToFile("===== DropGenericBox_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: DropGenericBox_NonBlocking - No control handle! Did you call AcquireControl()?");
        return;
    }

    // 2. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue DropGenericBox Command ---
        if (pVTable->DropGenericBox) {
            LogToFile("    [DropGenericBox_NonBlocking] Issuing command...");
            try {
                pVTable->DropGenericBox(pExpert);
                LogToFile("    [DropGenericBox_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during DropGenericBox_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: DropGenericBox function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== DropGenericBox_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::FollowPreCalculatedRoute_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spRoute,
    int moveType, sol::optional<bool> avoidObstacles,
    sol::optional<bool> ignorePathPreferability) {
    LogToFile("===== FollowPreCalculatedRoute_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: FollowPreCalculatedRoute_NonBlocking - No control handle! Did you call AcquireControl()?");
        return;
    }

    CScriptThing* pRoute = spRoute.get();

    // 2. Input Validation
    if (!pRoute) {
        LogToFile("!!! ERROR: FollowPreCalculatedRoute_NonBlocking - pRoute is NULL!");
        return;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Clear existing commands ---
        if (pVTable->ClearCommands) {
            LogToFile("    [FollowPreCalculatedRoute_NonBlocking] Clearing existing commands...");
            pVTable->ClearCommands(pExpert);
        }

        // --- Issue FollowPreCalculatedRoute Command ---
        if (pVTable->FollowPreCalculatedRoute) {
            LogToFile("    [FollowPreCalculatedRoute_NonBlocking] Issuing command...");
            EScriptEntityMoveType eMoveType = static_cast<EScriptEntityMoveType>(moveType);
            const CScriptThing* pConstRoute = pRoute;

            // Parameters based on function signature
            bool b1 = avoidObstacles.value_or(true);
            bool b2 = ignorePathPreferability.value_or(false);

            try {
                pVTable->FollowPreCalculatedRoute(pExpert, pConstRoute, eMoveType, b1, b2);
                LogToFile("    [FollowPreCalculatedRoute_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during FollowPreCalculatedRoute_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: FollowPreCalculatedRoute function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== FollowPreCalculatedRoute_NonBlocking Wrapper END =====");
}

bool LuaEntityAPI::IsFollowActionRunning_NonBlocking(CScriptThing* pMe, sol::object target_obj) {
    // LogToFile("===== IsFollowActionRunning_NonBlocking Wrapper START ====="); // Too spammy for a check function

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        // LogToFile("!!! ERROR: IsFollowActionRunning_NonBlocking - No control handle!");
        return false;
    }

    // 2. Parse target from Lua (can be shared_ptr or raw pointer)
    CScriptThing* rawTarget = nullptr;
    if (target_obj.is<std::shared_ptr<CScriptThing>>()) {
        rawTarget = target_obj.as<std::shared_ptr<CScriptThing>>().get();
    }
    else if (target_obj.is<CScriptThing*>()) {
        rawTarget = target_obj.as<CScriptThing*>();
    }

    if (!rawTarget) {
        // LogToFile("!!! ERROR: IsFollowActionRunning_NonBlocking - Invalid target type!");
        return false;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable && pVTable->IsFollowActionRunning) {
        try {
            const CScriptThing* pConstTarget = rawTarget;
            return pVTable->IsFollowActionRunning(pExpert, pConstTarget);
        }
        catch (...) {
            // LogToFile("!!! EXCEPTION during IsFollowActionRunning_NonBlocking call!");
            return false;
        }
    }

    // LogToFile("!!! ERROR: Failed to get valid Expert/VTable or function pointer null!");
    return false;
}

void LuaEntityAPI::Wait_NonBlocking(CScriptThing* pMe, float seconds) {
    LogToFile("===== Wait_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: Wait_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== Wait_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue Wait Command ---
        if (pVTable->Wait) {
            LogToFile("    [Wait_NonBlocking] Issuing command to wait for " + std::to_string(seconds) + " seconds...");
            try {
                pVTable->Wait(pExpert, seconds);
                LogToFile("    [Wait_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during Wait_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: Wait function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== Wait_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::WaitForEntityToFinishPerformingTasks_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetEntity) {
    LogToFile("===== WaitForEntityToFinishPerformingTasks_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: WaitForEntityToFinishPerformingTasks_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== WaitForEntityToFinishPerformingTasks_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Input Validation
    CScriptThing* pTargetEntity = spTargetEntity.get();
    if (!pTargetEntity || !pTargetEntity->pImp.Data) {
        LogToFile("!!! ERROR: WaitForEntityToFinishPerformingTasks_NonBlocking - pTargetEntity is NULL or has no pImp.Data!");
        LogToFile("===== WaitForEntityToFinishPerformingTasks_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue WaitForEntityToFinishPerformingTasks Command ---
        if (pVTable->WaitForEntityToFinishPerformingTasks) {
            LogToFile("    [WaitForEntityToFinishPerformingTasks_NonBlocking] Issuing command to wait for target entity...");

            // The API expects the base object pointer from the target entity's pImp
            auto* pTargetHandle = reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(pTargetEntity->pImp.Data);

            try {
                pVTable->WaitForEntityToFinishPerformingTasks(pExpert, pTargetHandle);
                LogToFile("    [WaitForEntityToFinishPerformingTasks_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during WaitForEntityToFinishPerformingTasks_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: WaitForEntityToFinishPerformingTasks function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== WaitForEntityToFinishPerformingTasks_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::ClearAllActions_NonBlocking(CScriptThing* pMe) {
    SelectControlHandle(pMe);
    LogToFile("===== ClearAllActions_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: ClearAllActions_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== ClearAllActions_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue ClearAllActions Command ---
        if (pVTable->ClearAllActions) {
            LogToFile("    [ClearAllActions_NonBlocking] Issuing command...");
            try {
                pVTable->ClearAllActions(pExpert);
                LogToFile("    [ClearAllActions_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during ClearAllActions_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: ClearAllActions function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== ClearAllActions_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::ClearAllActionsIncludingLoopingAnimations_NonBlocking(CScriptThing* pMe) {
    LogToFile("===== ClearAllActionsIncludingLoopingAnimations_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: ClearAllActionsIncludingLoopingAnimations_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== ClearAllActionsIncludingLoopingAnimations_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue ClearAllActionsIncludingLoopingAnimations Command ---
        if (pVTable->ClearAllActionsIncludingLoopingAnimations) {
            LogToFile("    [ClearAllActionsIncludingLoopingAnimations_NonBlocking] Issuing command...");
            try {
                pVTable->ClearAllActionsIncludingLoopingAnimations(pExpert);
                LogToFile("    [ClearAllActionsIncludingLoopingAnimations_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during ClearAllActionsIncludingLoopingAnimations_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: ClearAllActionsIncludingLoopingAnimations function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== ClearAllActionsIncludingLoopingAnimations_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::UnsheatheWeapons_NonBlocking(CScriptThing* pMe) {
    LogToFile("===== UnsheatheWeapons_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: UnsheatheWeapons_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== UnsheatheWeapons_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue UnsheatheWeapons Command ---
        if (pVTable->UnsheatheWeapons) {
            LogToFile("    [UnsheatheWeapons_NonBlocking] Issuing command...");
            try {
                pVTable->UnsheatheWeapons(pExpert);
                LogToFile("    [UnsheatheWeapons_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during UnsheatheWeapons_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: UnsheatheWeapons function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== UnsheatheWeapons_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::SummonerLightningOrbAttackTarget_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing) {
    LogToFile("===== SummonerLightningOrbAttackTarget_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: SummonerLightningOrbAttackTarget_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== SummonerLightningOrbAttackTarget_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Input Validation
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        LogToFile("!!! ERROR: SummonerLightningOrbAttackTarget_NonBlocking - pTargetThing is NULL!");
        LogToFile("===== SummonerLightningOrbAttackTarget_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue SummonerLightningOrbAttackTarget Command ---
        if (pVTable->SummonerLightningOrbAttackTarget) {
            LogToFile("    [SummonerLightningOrbAttackTarget_NonBlocking] Issuing command...");
            try {
                // The API expects a const CScriptThing*
                pVTable->SummonerLightningOrbAttackTarget(pExpert, pTargetThing);
                LogToFile("    [SummonerLightningOrbAttackTarget_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during SummonerLightningOrbAttackTarget_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: SummonerLightningOrbAttackTarget function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== SummonerLightningOrbAttackTarget_NonBlocking Wrapper END =====");
}

void LuaEntityAPI::FireProjectileWeaponAtTarget_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing) {
    LogToFile("===== FireProjectileWeaponAtTarget_NonBlocking Wrapper START =====");

    // 1. Check if we have the handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: FireProjectileWeaponAtTarget_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== FireProjectileWeaponAtTarget_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 2. Input Validation
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        LogToFile("!!! ERROR: FireProjectileWeaponAtTarget_NonBlocking - pTargetThing is NULL!");
        LogToFile("===== FireProjectileWeaponAtTarget_NonBlocking Wrapper END (Failure) =====");
        return;
    }

    // 3. Get Expert and VTable
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue FireProjectileWeaponAtTarget Command ---
        if (pVTable->FireProjectileWeaponAtTarget) {
            LogToFile("    [FireProjectileWeaponAtTarget_NonBlocking] Issuing command...");
            try {
                // The API expects a const CScriptThing*
                pVTable->FireProjectileWeaponAtTarget(pExpert, pTargetThing);
                LogToFile("    [FireProjectileWeaponAtTarget_NonBlocking] ...Command issued successfully.");
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during FireProjectileWeaponAtTarget_NonBlocking call!");
            }
        }
        else {
            LogToFile("!!! ERROR: FireProjectileWeaponAtTarget function pointer is null!");
        }
    }
    else {
        LogToFile("!!! ERROR: Failed to get valid Expert object or derived VTable!");
    }

    LogToFile("===== FireProjectileWeaponAtTarget_NonBlocking Wrapper END =====");
}

bool LuaEntityAPI::Speak_Blocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, const std::string& dialogueKey,
    int selectionMethod, bool makeTargetListen, bool soundIn2D, bool overScreenFade)
{
    SelectControlHandle(pMe);
    LogToFile("===== Speak_Blocking Wrapper START =====");
    std::stringstream ss_log;

    // 1. Check if we have the persistent handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: Speak_Blocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== Speak_Blocking Wrapper END (Failure: No Handle) =====");
        return false;
    }

    // 2. Check prerequisites for the blocking wait (matching SpeakAndWait/MoveToPosition)
    //    Ensure IsActiveThreadTerminating_API is available, as used in the working examples.
    const bool hasTerminationAPI = (m_pEntityHost && IsActiveThreadTerminating_Entity_API) ||
        (m_pQuestHost && IsActiveThreadTerminating_Quest_API);
    if (!NewScriptFrame_API || !hasTerminationAPI) {
        LogToFile("!!! ERROR: Speak_Blocking - Wait prerequisites not met (NewScriptFrame or termination API missing)!");
        LogToFile("===== Speak_Blocking Wrapper END (Failure: API Missing) =====");
        return false;
    }

    // 3. Input Validation for Target
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        LogToFile("!!! WARNING: Speak_Blocking - Target CScriptThing* is NULL. Proceeding, but game might require a target.");
    }

    // 4. Get Expert and VTable from our existing handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- Issue Speak Command ---
        if (pVTable->Speak) {
            LogToFile("    [Speak_Blocking] Issuing 'Speak' command...");
            FableString fsKey(dialogueKey.c_str());
            ETextGroupSelectionMethod eSelectMethod = static_cast<ETextGroupSelectionMethod>(selectionMethod);

            ss_log.str(""); // Clear log stream
            ss_log << "  Parameters:"
                << "\n    Target: 0x" << std::hex << pTargetThing
                << "\n    Key: " << dialogueKey
                << "\n    Method: " << std::dec << selectionMethod
                << "\n    Listen: " << (makeTargetListen ? "true" : "false")
                << "\n    2D Sound: " << (soundIn2D ? "true" : "false")
                << "\n    Over Fade: " << (overScreenFade ? "true" : "false");
            LogToFile(ss_log.str());


            try {
                // Call the NON-BLOCKING VTable function using the const char* from the FableString
                pVTable->Speak(pExpert, pTargetThing, CCharString_ToConstChar_API(fsKey.get()), eSelectMethod,
                    makeTargetListen, soundIn2D, overScreenFade);
                LogToFile("    [Speak_Blocking] ...'Speak' command issued successfully.");

                // --- BLOCKING Wait Loop (Matching SpeakAndWait/MoveToPosition) ---
                if (pVTable->IsPerformingScriptTask) {
                    LogToFile("    [Speak_Blocking] Entering wait loop...");
                    int waitFrames = 0;
                    // Added pExpert check in loop condition for extra safety, like SpeakAndWait
                    while (pExpert && pVTable->IsPerformingScriptTask(pExpert)) {
                        NewScriptFrame_API(m_pGameInterface); // Yield frame
                        waitFrames++;

                        // Retail entity scripts call 0x00F35B30 with their entity-script object,
                        // not the quest-interface vtable termination predicate.
                        if (IsThreadTerminating()) {
                            LogToFile("    [Speak_Blocking] ...Thread terminating during speech wait (waited " + std::to_string(waitFrames) + " frames).");
                            return false;
                        }
                    }
                    // Check if the loop exited due to termination or completion
                    if (!IsThreadTerminating()) {
                        LogToFile("    [Speak_Blocking] ...Wait loop finished after " + std::to_string(waitFrames) + " frames.");
                    }
                }
                else {
                    LogToFile("!!! WARNING: Speak_Blocking - IsPerformingScriptTask function pointer is null! Cannot block.");
                }
                // --- End BLOCKING Wait Loop ---
                return true;
            }
            catch (...) {
                LogToFile("!!! EXCEPTION during Speak_Blocking call or wait loop!");
                return false;
            }
        }
        else {
            LogToFile("!!! ERROR: Speak_Blocking - Speak function pointer is null in derived VTable!");
        }
    }
    else {
        LogToFile("!!! ERROR: Speak_Blocking - Failed to get valid Expert object or derived VTable from handle!");
    }

    LogToFile("===== Speak_Blocking Wrapper END =====");
    return false;
}

void LuaEntityAPI::Converse_NonBlocking(CScriptThing* pMe, const std::shared_ptr<CScriptThing>& spTargetThing, const std::string& dialogueKey) {
    LogToFile("===== Converse_NonBlocking (Calling Speak) Wrapper START ====="); // Renamed log
    std::stringstream ss_log;

    // 1. Check our handle
    if (!m_pControlHandle || !m_pControlHandle->pImp.Data) {
        LogToFile("!!! ERROR: Converse_NonBlocking - No control handle! Did you call AcquireControl()?");
        LogToFile("===== Converse_NonBlocking (Calling Speak) Wrapper END (Failure: No Handle) =====");
        return;
    }

    // 2. Validate Target Pointer
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        LogToFile("!!! ERROR: Converse_NonBlocking - Target CScriptThing* shared_ptr contained NULL!");
        LogToFile("===== Converse_NonBlocking (Calling Speak) Wrapper END (Failure: Null Target Pointer) =====");
        return;
    }

    // 3. Validate Target Object Validity using IsNull
    bool targetIsNull = true;
    if (pTargetThing->pVTable) {
        CScriptThingVTable* pTargetVTable = reinterpret_cast<CScriptThingVTable*>(pTargetThing->pVTable);
        if (pTargetVTable->IsNull) {
            try { targetIsNull = pTargetVTable->IsNull(pTargetThing); }
            catch (...) { targetIsNull = true; }
        }
        else { targetIsNull = true; }
    }
    else { targetIsNull = true; }

    if (targetIsNull) {
        LogToFile("!!! ERROR: Converse_NonBlocking - Target CScriptThing* is logically NULL or invalid!");
        LogToFile("===== Converse_NonBlocking (Calling Speak) Wrapper END (Failure: Invalid Target) =====");
        return;
    }

    // 4. Get Expert and VTable from OUR handle
    CScriptGameResourceObjectScriptedThing* pExpert = reinterpret_cast<CScriptGameResourceObjectScriptedThing*>(m_pControlHandle->pImp.Data);
    CScriptGameResourceObjectScriptedThingVTable* pVTable = pExpert ?
        reinterpret_cast<CScriptGameResourceObjectScriptedThingVTable*>(pExpert->pVTable) : nullptr;

    if (pExpert && pVTable) {
        // --- ADDED: ClearCommands ---
        if (pVTable->ClearCommands) {
            LogToFile("    [Converse_NonBlocking] Clearing existing commands...");
            try { pVTable->ClearCommands(pExpert); }
            catch (...) { LogToFile("!!! EXCEPTION during ClearCommands in Converse_NonBlocking!"); }
        }
        else {
            LogToFile("!!! WARNING: Converse_NonBlocking - ClearCommands function pointer is null!");
        }
        // --- END ADDED ClearCommands ---

        // 5. Check Speak VTable function pointer (since we are calling Speak now)
        if (pVTable->Speak) {
            LogToFile("    [Converse_NonBlocking] Issuing 'Speak' command (mimicking Converse)...");
            FableString fsKey(dialogueKey.c_str());

            // --- Set Default Parameters suitable for Converse ---
            ETextGroupSelectionMethod eSelectMethod = GROUP_SELECT_RANDOM_NO_REPEAT; // Sensible default
            bool makeTargetListen = false; // Typically false for NPC-NPC?
            bool soundIn2D = true;        // Usually true for dialogue
            bool overScreenFade = false;   // Usually false

            ss_log.str("");
            ss_log << "  Speak Parameters (Defaults for Converse):"
                << "\n    Target CScriptThing*: 0x" << std::hex << pTargetThing // Passing CScriptThing* now
                << "\n    Key: " << dialogueKey
                << "\n    Method: " << std::dec << eSelectMethod
                << "\n    Listen: " << (makeTargetListen ? "true" : "false")
                << "\n    2D Sound: " << (soundIn2D ? "true" : "false")
                << "\n    Over Fade: " << (overScreenFade ? "true" : "false");
            LogToFile(ss_log.str());

            try {
                // 7. Call Speak VTable function with CScriptThing* target
                pVTable->Speak(pExpert, pTargetThing, CCharString_ToConstChar_API(fsKey.get()), eSelectMethod,
                    makeTargetListen, soundIn2D, overScreenFade);
                LogToFile("    [Converse_NonBlocking] ...'Speak' command issued successfully.");

            }
            catch (...) {
                LogToFile("!!! EXCEPTION during Speak call (within Converse_NonBlocking)!");
            }
        }
        else {
            // This is critical - if Speak is null, we can't mimic Converse
            LogToFile("!!! ERROR: Converse_NonBlocking - Speak function pointer is null in VTable! Cannot mimic Converse.");
        }
    }
    else {
        LogToFile("!!! ERROR: Converse_NonBlocking - Failed to get valid Expert object or derived VTable from our handle!");
    }

    // 8. Return immediately
    LogToFile("===== Converse_NonBlocking (Calling Speak) Wrapper END =====");
}

std::string LuaEntityAPI::GetDefName(CScriptThing* pMe) {
    // Pattern from GetDataString
    if (!pMe || !pMe->pVTable) return "";
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;

    if (!pVTable->GetDefName) return ""; // Check if function pointer is valid

    CCharString result = { 0 };
    pVTable->GetDefName(pMe, &result);

    // Pattern from GetDataString
    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result); // Ensure cleanup even if text is null
    }
    return "";
}

sol::table LuaEntityAPI::GetHomePos(CScriptThing* pMe, sol::this_state s) {
    // Pattern from GetPos
    sol::state_view lua(s);
    sol::table pos_table = lua.create_table();

    if (!pMe || !pMe->pVTable) return pos_table;
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;

    if (!pVTable->GetHomePos) return pos_table; // Check if function pointer is valid

    C3DVector resultPos = { 0 };
    // Call the function
    pVTable->GetHomePos(pMe, &resultPos);

    // Populate table
    pos_table["x"] = resultPos.x;
    pos_table["y"] = resultPos.y;
    pos_table["z"] = resultPos.z;

    return pos_table;
}

std::string LuaEntityAPI::GetCurrentMapName(CScriptThing* pMe) {
    // Pattern from GetDataString
    if (!pMe || !pMe->pVTable) return "";
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;

    if (!pVTable->GetCurrentMapName) return ""; // Check if function pointer is valid

    CCharString result = { 0 };
    pVTable->GetCurrentMapName(pMe, &result);

    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result);
    }
    return "";
}

std::string LuaEntityAPI::GetHomeMapName(CScriptThing* pMe) {
    // Pattern from GetDataString
    if (!pMe || !pMe->pVTable) return "";
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;

    if (!pVTable->GetHomeMapName) return ""; // Check if function pointer is valid

    CCharString result = { 0 };
    pVTable->GetHomeMapName(pMe, &result);

    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result);
    }
    return "";
}

bool LuaEntityAPI::IsSneaking(CScriptThing* pMe) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = (CScriptThingVTable*)pMe->pVTable;

    if (!pVTable->IsSneaking) return false; // Check if function pointer is valid

    return pVTable->IsSneaking(pMe);
}

std::shared_ptr<CScriptThing> LuaEntityAPI::MsgWhoKilledMe(CScriptThing* pMe) {
    if (!pMe) return nullptr;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return nullptr;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgWhoKilledMe) return nullptr;

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return nullptr;
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    pVTable->MsgWhoKilledMe(pImp, pResultBuffer);

    return WrapScriptThingOutput(pResultBuffer);
}

std::shared_ptr<CScriptThing> LuaEntityAPI::MsgWhoHitMe(CScriptThing* pMe) {
    if (!pMe) return nullptr;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return nullptr;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgWhoHitMe) return nullptr;

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return nullptr;
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    pVTable->MsgWhoHitMe(pImp, pResultBuffer);

    return WrapScriptThingOutput(pResultBuffer);
}

bool LuaEntityAPI::MsgIsHitByHeroWithFlourish(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsHitByWithFlourish) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsHitByWithFlourish(pImp, heroName);
}

bool LuaEntityAPI::MsgIsHitByHeroWithDecapitate(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsHitByWithDecapitate) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsHitByWithDecapitate(pImp, heroName);
}

bool LuaEntityAPI::MsgIsHitByHeroWithWeapon(CScriptThing* pMe, const std::string& weaponName) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsHitByWithWeapon) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    FableString fsWeapon(weaponName.c_str());
    return pVTable->MsgIsHitByWithWeapon(pImp, heroName, fsWeapon);
}

namespace {
    struct HitEventTimeFilter {
        const unsigned int* createdAfter;
        const unsigned int* createdBeforeOrOn;
        const CCharString* hitterScriptName;
    };

    typedef bool(__thiscall* tGetHitEvent)(
        void* entityEvents,
        void** outEvent,
        const HitEventTimeFilter* filter);

    bool IsReadableRange(const void* address, size_t bytes) {
        if (!address || bytes == 0) return false;
        MEMORY_BASIC_INFORMATION info = {};
        if (!VirtualQuery(address, &info, sizeof(info))) return false;
        if (info.State != MEM_COMMIT || (info.Protect & (PAGE_NOACCESS | PAGE_GUARD))) return false;
        const uintptr_t begin = reinterpret_cast<uintptr_t>(address);
        const uintptr_t regionEnd = reinterpret_cast<uintptr_t>(info.BaseAddress) + info.RegionSize;
        return begin <= regionEnd && bytes <= regionEnd - begin;
    }

    const char* GetAugmentationTypeName(int type) {
        switch (type) {
        case 0x001: return "Sharpening";
        case 0x002: return "Silver";
        case 0x004: return "Flame";
        case 0x008: return "Lightning";
        case 0x010: return "Piercing";
        case 0x020: return "Health";
        case 0x040: return "Mana";
        case 0x080: return "Experience";
        case 0x100: return "HobbeKiller";
        case 0x200: return "BanditSlayer";
        default: return nullptr;
        }
    }
}

sol::object LuaEntityAPI::MsgGetHitByWeaponAugmentations(
    CScriptThing* pMe,
    const std::string& hitterName,
    sol::this_state s) {
    sol::state_view lua(s);
    auto failure = [&](const char* reason) -> sol::object {
        sol::table result = lua.create_table();
        result["found"] = false;
        result["reason"] = reason;
        return sol::make_object(lua, result);
    };
    if (!pMe) return failure("script_thing");

    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return failure("script_implementation");
    CGameScriptThingVTable* scriptVTable =
        reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);
    if (!scriptVTable->GetPThing) return failure("get_pthing_api");
    CThing* nativeThing = scriptVTable->GetPThing(pImp);
    if (!nativeThing) return failure("native_thing");

    // CThing's interface vector-map is [begin,end] at +0x44/+0x48. Each entry
    // is { ETCInterfaceType, CTCBase* }; retail asks for CTCEntityEvents (0x42).
    unsigned char* thing = reinterpret_cast<unsigned char*>(nativeThing);
    int* interfacesBegin = *reinterpret_cast<int**>(thing + 0x44);
    int* interfacesEnd = *reinterpret_cast<int**>(thing + 0x48);
    const uintptr_t interfacesBeginAddress = reinterpret_cast<uintptr_t>(interfacesBegin);
    const uintptr_t interfacesEndAddress = reinterpret_cast<uintptr_t>(interfacesEnd);
    if (!interfacesBegin || !interfacesEnd || interfacesBeginAddress > interfacesEndAddress ||
        (interfacesEndAddress - interfacesBeginAddress) % (2 * sizeof(int)) != 0 ||
        interfacesEndAddress - interfacesBeginAddress > 256 * 2 * sizeof(int) ||
        !IsReadableRange(interfacesBegin, interfacesEndAddress - interfacesBeginAddress)) {
        return failure("interface_map");
    }

    void* entityEvents = nullptr;
    for (int* entry = interfacesBegin; entry + 1 < interfacesEnd; entry += 2) {
        if (entry[0] == 0x42) {
            entityEvents = reinterpret_cast<void*>(entry[1]);
            break;
        }
        if (entry[0] > 0x42) break;
    }
    if (!entityEvents) return failure("entity_events");

    // Match CGameScriptThing::MsgIsHitBy's active-script time window exactly.
    void* questManager = *ASLR<void**>(0x013B89FC);
    if (!questManager) return failure("quest_manager");
    void* active = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(questManager) + 0x88);
    if (!active) return failure("active_script");
    void* scriptBase = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(active) + 0x08);
    if (!scriptBase) return failure("script_base");
    void* spawned = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(scriptBase) + 0x2c);
    if (!spawned) return failure("spawned_function");

    const unsigned int* createdAfter = reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(spawned) + 0x18);
    const unsigned int* createdBefore = reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(spawned) + 0x1c);
    // Match retail MsgIsHitBy's Bind3 filter: createdAfter, createdBeforeOrOn,
    // then CIsHitEventHitByThingWithScriptName. Earlier live misses happened
    // before this call because PThing was read through the wrong local layout;
    // the predicate itself must be tested with virtual GetPThing resolution.
    FableString hitter(hitterName.c_str());
    HitEventTimeFilter filter = { createdAfter, createdBefore, hitter };

    void* eventHitBy = nullptr;
    tGetHitEvent getEvent = ASLR<tGetHitEvent>(0x008D4840);
    if (!getEvent(entityEvents, &eventHitBy, &filter) || !eventHitBy) {
        return failure("get_event");
    }

    // CEventHitBase +0x08 is CCountedPointer<CHitParameters>. Copy the vector;
    // never return pointers owned by the event queue to Lua.
    void* hitParameters = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(eventHitBy) + 0x08);
    if (!IsReadableRange(hitParameters, 0x4c)) return failure("hit_parameters");
    // Retail VC7.1 CHitParameters stores vector begin/end/capacity at
    // +0x3c/+0x40/+0x44. This is proven by the constructor at 0x00789238
    // and copy constructor at 0x00812170. FableWin's newer debug STL adds a
    // container proxy and therefore has a different layout.
    int** vector = reinterpret_cast<int**>(reinterpret_cast<unsigned char*>(hitParameters) + 0x3c);
    int* begin = vector[0];
    int* end = vector[1];
    int* capacity = vector[2];
    const uintptr_t beginAddress = reinterpret_cast<uintptr_t>(begin);
    const uintptr_t endAddress = reinterpret_cast<uintptr_t>(end);
    const uintptr_t capacityAddress = reinterpret_cast<uintptr_t>(capacity);
    if ((!begin && (end || capacity)) || beginAddress > endAddress || endAddress > capacityAddress ||
        (endAddress - beginAddress) % sizeof(int) != 0) {
        return failure("vector_layout");
    }
    const size_t count = begin ? (endAddress - beginAddress) / sizeof(int) : 0;
    if (count > 32 || (count && !IsReadableRange(begin, count * sizeof(int)))) {
        return failure("vector_range");
    }

    sol::table result = lua.create_table();
    sol::table typeIds = lua.create_table(static_cast<int>(count), 0);
    sol::table types = lua.create_table(static_cast<int>(count), 0);
    unsigned int augmentationMask = 0;
    for (size_t index = 0; index < count; ++index) {
        const int type = begin[index];
        const char* typeName = GetAugmentationTypeName(type);
        typeIds[static_cast<int>(index + 1)] = type;
        types[static_cast<int>(index + 1)] = typeName
            ? std::string(typeName)
            : std::string("Unknown(") + std::to_string(type) + ")";
        augmentationMask |= static_cast<unsigned int>(type);
    }
    result["found"] = true;
    result["types"] = types;
    result["typeIds"] = typeIds;
    result["augmentationMask"] = augmentationMask;
    return sol::make_object(lua, result);
}

sol::object LuaEntityAPI::MsgIsHitByHeroWithProjectileWeapon(CScriptThing* pMe, sol::this_state s) {
    sol::state_view lua(s);
    if (!pMe) return sol::make_object(lua, sol::nil);
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return sol::make_object(lua, sol::nil);
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsHitByWithProjectileWeapon) return sol::make_object(lua, sol::nil);

    FableString heroName("SCRIPT_NAME_HERO");
    float damageDealt = 0.0f; // Output parameter

    bool result = pVTable->MsgIsHitByWithProjectileWeapon(pImp, heroName, &damageDealt);

    if (result) {
        return sol::make_object(lua, damageDealt);
    }

    return sol::make_object(lua, sol::nil);
}

bool LuaEntityAPI::MsgIsUsedByHero(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsUsedBy) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsUsedBy(pImp, heroName);
}

bool LuaEntityAPI::MsgIsTriggeredByHero(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsTriggeredBy) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsTriggeredBy(pImp, heroName);
}

bool LuaEntityAPI::MsgIsKnockedOutByHero(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsKnockedOutBy) return false;

    FableString heroName("SCRIPT_NAME_HERO");
    return pVTable->MsgIsKnockedOutBy(pImp, heroName);
}

bool LuaEntityAPI::MsgPerformedSpecialAbility(CScriptThing* pMe, int abilityEnum) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgPerformedSpecialAbility) return false;

    EHeroAbility eAbility = static_cast<EHeroAbility>(abilityEnum);
    return pVTable->MsgPerformedSpecialAbility(pImp, eAbility);
}

bool LuaEntityAPI::MsgPerformedAnySpecialAbility(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgPerformedAnySpecialAbility) return false;

    return pVTable->MsgPerformedAnySpecialAbility(pImp);
}

bool LuaEntityAPI::MsgPerformedAnyAggressiveSpecialAbility(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgPerformedAnyAggressiveSpecialAbility) return false;

    return pVTable->MsgPerformedAnyAggressiveSpecialAbility(pImp);
}

bool LuaEntityAPI::MsgPerformedAnyNonAggressiveSpecialAbility(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgPerformedAnyNonAggressiveSpecialAbility) return false;

    return pVTable->MsgPerformedAnyNonAggressiveSpecialAbility(pImp);
}

bool LuaEntityAPI::MsgReceivedInventoryItem(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgReceivedInventoryItem) return false;

    return pVTable->MsgReceivedInventoryItem(pImp);
}

bool LuaEntityAPI::MsgIsHitByHeroSpecialAbility(CScriptThing* pMe, int abilityEnum) {
    if (!pMe || !pMe->pVTable) return false;
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pMe->pVTable);
    if (!pVTable->MsgIsHitBySpecialAbilityFrom) return false;

    EHeroAbility eAbility = static_cast<EHeroAbility>(abilityEnum);
    FableString heroName("SCRIPT_NAME_HERO");
    const bool hit = pVTable->MsgIsHitBySpecialAbilityFrom(pMe, eAbility, heroName.get());
    if (hit) {
        LogToFile("    [HitEvidence] MsgIsHitBySpecialAbilityFrom(" + std::to_string(abilityEnum) +
                  ", SCRIPT_NAME_HERO) returned true.");
    }
    return hit;
}

bool LuaEntityAPI::MsgOpenedChest(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgOpenedChest) return false;

    return pVTable->MsgOpenedChest(pImp);
}

bool LuaEntityAPI::MsgIsKicked(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->MsgIsKicked) return false;

    return pVTable->MsgIsKicked(pImp);
}

bool LuaEntityAPI::IsUnconscious(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->IsUnconscious) return false;

    return pVTable->IsUnconscious(pImp);
}

bool LuaEntityAPI::IsOpenDoor(CScriptThing* pMe) {
    if (!pMe) return false;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return false;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->IsOpenDoor) return false;

    return pVTable->IsOpenDoor(pImp);
}

void LuaEntityAPI::SetAsUsable(CScriptThing* pMe, bool isUsable) {
    if (!pMe) return;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->SetAsUsable) return;

    pVTable->SetAsUsable(pImp, isUsable);
}

void LuaEntityAPI::SetFriendsWithEverythingFlag(CScriptThing* pMe, bool isFriends) {
    if (!pMe) return;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->SetFriendsWithEverythingFlag) return;

    pVTable->SetFriendsWithEverythingFlag(pImp, isFriends);
}

void LuaEntityAPI::SetActivationTriggerStatus(CScriptThing* pMe, bool isActive) {
    if (!pMe) return;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->SetActivationTriggerStatus) return;

    pVTable->SetActivationTriggerStatus(pImp, isActive);
}

void LuaEntityAPI::SetToKillOnLevelUnload(CScriptThing* pMe, bool shouldKill) {
    if (!pMe) return;
    CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);
    if (!pImp || !pImp->pVTable) return;
    CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pImp->pVTable);

    if (!pVTable->SetToKillOnLevelUnload) return;

    pVTable->SetToKillOnLevelUnload(pImp, shouldKill);
}
