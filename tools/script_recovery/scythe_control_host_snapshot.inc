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

