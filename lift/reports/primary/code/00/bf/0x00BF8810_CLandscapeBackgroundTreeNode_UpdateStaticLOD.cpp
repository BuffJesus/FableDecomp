LS_LOADING_STATUS CLandscapeBackgroundTreeNode::UpdateStaticLOD(
    double distanceToCamera,
    bool forceUpdate,
    CEngineStaticMapFileBlockManager* pStaticMapFileBlockManager)
{
    static_assert(sizeof(void*) == 4);

    struct StaticLODEntry
    {
        std::uint32_t m_unknown00;
        std::uint32_t m_unknown04;
        std::uint8_t m_patchDetailLevel;
        std::byte m_pad09[0x07];
    };

    struct LoadedStaticLODBase;

    struct LoadedStaticLODVTable
    {
        void* m_slot00;
        void* m_slot04;
        void* m_slot08;
        void* m_slot0C;
        char(__thiscall* m_isPatchObject)(LoadedStaticLODBase*);
    };

    struct LoadedStaticLODBase
    {
        LoadedStaticLODVTable* m_vtable;
    };

    struct LoadedStaticLODPatch : LoadedStaticLODBase
    {
        std::byte m_pad04[0x114];
        std::uint8_t m_detailLevel;
    };

    struct LoadedStaticLODTree : LoadedStaticLODBase
    {
        CLandscapeBackgroundTreeNode m_firstChild;
        CLandscapeBackgroundTreeNode m_secondChild;
    };

    struct NodeRuntime
    {
        std::byte m_pad00[0x10];
        StaticLODEntry m_staticLODEntries[8];
        std::byte m_pad90[0x08];
        void* m_pendingLoadTask;
        std::byte m_pad9C[0x04];
        LoadedStaticLODBase* m_loadedStaticLOD;
        float m_minBoundsX;
        float m_minBoundsY;
        float m_minBoundsZ;
        float m_maxBoundsX;
        float m_maxBoundsY;
        float m_maxBoundsZ;
        std::uint32_t m_loadStateC0;
        std::uint32_t m_loadStateC4;
        std::uint32_t m_loadStateC8;
        std::uint32_t m_loadStateCC;
        std::byte m_padD0[0x10];
        std::uint8_t m_lodStateE0;
        std::uint8_t m_currentLODLevel;
        std::uint8_t m_requestedLODEntryIndex;
        std::uint8_t m_lodStateE3;
        std::byte m_padE4[0x02];
        std::uint8_t m_hasStaticLODData;
    };

    static_assert(sizeof(StaticLODEntry) == 0x10);
    static_assert(offsetof(StaticLODEntry, m_patchDetailLevel) == 0x08);
    static_assert(offsetof(LoadedStaticLODPatch, m_detailLevel) == 0x118);
    static_assert(offsetof(LoadedStaticLODTree, m_firstChild) == 0x04);
    static_assert(offsetof(LoadedStaticLODTree, m_secondChild) == 0xEC);
    static_assert(offsetof(NodeRuntime, m_staticLODEntries) == 0x10);
    static_assert(offsetof(NodeRuntime, m_pendingLoadTask) == 0x98);
    static_assert(offsetof(NodeRuntime, m_loadedStaticLOD) == 0xA0);
    static_assert(offsetof(NodeRuntime, m_minBoundsX) == 0xA8);
    static_assert(offsetof(NodeRuntime, m_minBoundsY) == 0xAC);
    static_assert(offsetof(NodeRuntime, m_minBoundsZ) == 0xB0);
    static_assert(offsetof(NodeRuntime, m_maxBoundsX) == 0xB4);
    static_assert(offsetof(NodeRuntime, m_maxBoundsY) == 0xB8);
    static_assert(offsetof(NodeRuntime, m_maxBoundsZ) == 0xBC);
    static_assert(offsetof(NodeRuntime, m_loadStateC0) == 0xC0);
    static_assert(offsetof(NodeRuntime, m_loadStateC4) == 0xC4);
    static_assert(offsetof(NodeRuntime, m_loadStateC8) == 0xC8);
    static_assert(offsetof(NodeRuntime, m_loadStateCC) == 0xCC);
    static_assert(offsetof(NodeRuntime, m_currentLODLevel) == 0xE1);
    static_assert(offsetof(NodeRuntime, m_requestedLODEntryIndex) == 0xE2);
    static_assert(offsetof(NodeRuntime, m_hasStaticLODData) == 0xE6);

    NodeRuntime& node = *reinterpret_cast<NodeRuntime*>(this);

    if (node.m_hasStaticLODData == 0)
    {
        return static_cast<LS_LOADING_STATUS>(1);
    }

    const double subFrameTimer = GFGetFastSubFrameTimer();
    if (distanceToCamera < subFrameTimer)
    {
        return static_cast<LS_LOADING_STATUS>(2);
    }

    node.m_loadStateC0 = 0;
    node.m_loadStateC4 = 0;
    node.m_loadStateCC = 0;
    node.m_loadStateC8 = 0;

    int foregroundRange = 8;
    unsigned int targetDetailLevel = 8;

    if (node.m_currentLODLevel < 8)
    {
        const bool inForegroundRange =
            reinterpret_cast<CEngineLandscapePatch*>(this)->DebugCalcInForgroundRange();
        foregroundRange = static_cast<int>(inForegroundRange);
        targetDetailLevel = node.m_staticLODEntries[foregroundRange].m_patchDetailLevel;
    }

    if (node.m_pendingLoadTask != nullptr)
    {
        const LS_LOADING_STATUS status =
            ProcessLoadTask(foregroundRange, distanceToCamera, forceUpdate, pStaticMapFileBlockManager);
        if (status != static_cast<LS_LOADING_STATUS>(1))
        {
            return status;
        }
    }

    LoadedStaticLODPatch* loadedPatch;
    if (node.m_loadedStaticLOD == nullptr ||
        node.m_loadedStaticLOD->m_vtable->m_isPatchObject(node.m_loadedStaticLOD) == '\0')
    {
        loadedPatch = nullptr;
    }
    else
    {
        loadedPatch = reinterpret_cast<LoadedStaticLODPatch*>(node.m_loadedStaticLOD);
    }

    LoadedStaticLODTree* loadedTree = nullptr;
    if (node.m_loadedStaticLOD != nullptr &&
        node.m_loadedStaticLOD->m_vtable->m_isPatchObject(node.m_loadedStaticLOD) == '\0')
    {
        loadedTree = reinterpret_cast<LoadedStaticLODTree*>(node.m_loadedStaticLOD);
    }

    if (loadedPatch == nullptr)
    {
        if (loadedTree == nullptr)
        {
            int planeIndex = 0;
            const float* plane = reinterpret_cast<const float*>(DAT_01436ea0 + 0x1c8);
            bool culled;

            do
            {
                const float x =
                    (plane[-2] <= _DAT_0122dedc) ? node.m_maxBoundsX : node.m_minBoundsX;
                const float y =
                    (plane[-1] <= _DAT_0122dedc) ? node.m_maxBoundsY : node.m_minBoundsY;
                const float z =
                    (*plane <= _DAT_0122dedc) ? node.m_maxBoundsZ : node.m_minBoundsZ;

                if (plane[1] < x * plane[-2] + y * plane[-1] + z * *plane)
                {
                    culled = false;
                    goto start_load;
                }

                ++planeIndex;
                plane += 4;
            } while (planeIndex < 4);

            culled = true;

        start_load:
            if (node.m_currentLODLevel < 8)
            {
                const bool startedLoad = StartPatchLoadTask(
                    node.m_staticLODEntries[node.m_requestedLODEntryIndex].m_patchDetailLevel,
                    pStaticMapFileBlockManager,
                    !culled);
                if (startedLoad)
                {
                    return ProcessLoadTask(
                        foregroundRange,
                        distanceToCamera,
                        forceUpdate,
                        pStaticMapFileBlockManager);
                }
            }
            else
            {
                const bool startedLoad = StartTreeLoadTask(pStaticMapFileBlockManager, !culled);
                if (startedLoad)
                {
                    return ProcessLoadTask(
                        foregroundRange,
                        distanceToCamera,
                        forceUpdate,
                        pStaticMapFileBlockManager);
                }
            }
        }
        else if (node.m_currentLODLevel < 8 &&
                 static_cast<int>(node.m_currentLODLevel) <= foregroundRange)
        {
            const bool startedLoad =
                StartPatchLoadTask(targetDetailLevel, pStaticMapFileBlockManager, false);
            if (startedLoad)
            {
                return ProcessLoadTask(
                    foregroundRange,
                    distanceToCamera,
                    forceUpdate,
                    pStaticMapFileBlockManager);
            }
        }
        else
        {
            const LS_LOADING_STATUS firstStatus =
                loadedTree->m_firstChild.UpdateStaticLOD(
                    distanceToCamera,
                    forceUpdate,
                    pStaticMapFileBlockManager);
            const LS_LOADING_STATUS secondStatus =
                loadedTree->m_secondChild.UpdateStaticLOD(
                    distanceToCamera,
                    forceUpdate,
                    pStaticMapFileBlockManager);

            if (firstStatus != static_cast<LS_LOADING_STATUS>(3) &&
                secondStatus != static_cast<LS_LOADING_STATUS>(3))
            {
                if (firstStatus != static_cast<LS_LOADING_STATUS>(2) &&
                    secondStatus != static_cast<LS_LOADING_STATUS>(2))
                {
                    return static_cast<LS_LOADING_STATUS>(1);
                }
                return static_cast<LS_LOADING_STATUS>(2);
            }
        }
    }
    else if (foregroundRange < static_cast<int>(node.m_currentLODLevel))
    {
        const bool startedLoad = StartTreeLoadTask(pStaticMapFileBlockManager, false);
        if (startedLoad)
        {
            return ProcessLoadTask(
                foregroundRange,
                distanceToCamera,
                forceUpdate,
                pStaticMapFileBlockManager);
        }
    }
    else if (targetDetailLevel < loadedPatch->m_detailLevel)
    {
        const bool startedLoad =
            StartPatchLoadTask(targetDetailLevel, pStaticMapFileBlockManager, false);
        if (startedLoad)
        {
            return ProcessLoadTask(
                foregroundRange,
                distanceToCamera,
                forceUpdate,
                pStaticMapFileBlockManager);
        }
    }
    else
    {
        if (targetDetailLevel <= loadedPatch->m_detailLevel)
        {
            return static_cast<LS_LOADING_STATUS>(1);
        }

        const bool startedLoad =
            StartPatchLoadTask(targetDetailLevel, pStaticMapFileBlockManager, false);
        if (startedLoad)
        {
            return ProcessLoadTask(
                foregroundRange,
                distanceToCamera,
                forceUpdate,
                pStaticMapFileBlockManager);
        }
    }

    return static_cast<LS_LOADING_STATUS>(3);
}