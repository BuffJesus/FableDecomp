#include "candidates/background_tree_static_lod_compiled.h"

LS_LOADING_STATUS CLandscapeBackgroundTreeNode::UpdateStaticLOD(
    double distanceToCamera,
    bool forceUpdate,
    CEngineStaticMapFileBlockManager* manager)
{
    if (HasStaticLODData == 0)
        return LS_LOADING_READY;
    if (distanceToCamera < GFGetFastSubFrameTimer())
        return LS_LOADING_PENDING;

    LoadStateC0 = 0;
    LoadStateC4 = 0;
    LoadStateC8 = 0;
    LoadStateCC = 0;

    int foregroundRange = 8;
    unsigned long targetDetailLevel = 8;
    if (CurrentLODLevel < 8)
    {
        foregroundRange = DebugCalcInForgroundRange();
        targetDetailLevel = StaticLODEntries[foregroundRange].PatchDetailLevel;
    }

    if (PendingLoadTask != 0)
    {
        LS_LOADING_STATUS status = ProcessLoadTask(
            foregroundRange, distanceToCamera, forceUpdate, manager);
        if (status != LS_LOADING_READY)
            return status;
    }

    CLoadedStaticLODPatch* loadedPatch = 0;
    CLoadedStaticLODTree* loadedTree = 0;
    if (LoadedStaticLOD != 0)
    {
        if (LoadedStaticLOD->IsPatchObject())
            loadedPatch = static_cast<CLoadedStaticLODPatch*>(LoadedStaticLOD);
        else
            loadedTree = static_cast<CLoadedStaticLODTree*>(LoadedStaticLOD);
    }

    if (loadedPatch != 0)
    {
        if (foregroundRange < CurrentLODLevel)
        {
            if (StartTreeLoadTask(manager, false))
                return ProcessLoadTask(foregroundRange, distanceToCamera, forceUpdate, manager);
        }
        else if (targetDetailLevel == loadedPatch->DetailLevel)
        {
            return LS_LOADING_READY;
        }
        else if (StartPatchLoadTask(targetDetailLevel, manager, false))
        {
            return ProcessLoadTask(foregroundRange, distanceToCamera, forceUpdate, manager);
        }
        return LS_LOADING_FAILED;
    }

    if (loadedTree != 0)
    {
        if (CurrentLODLevel < 8 && CurrentLODLevel <= foregroundRange)
        {
            if (StartPatchLoadTask(targetDetailLevel, manager, false))
                return ProcessLoadTask(foregroundRange, distanceToCamera, forceUpdate, manager);
            return LS_LOADING_FAILED;
        }

        LS_LOADING_STATUS firstStatus = loadedTree->FirstChild.UpdateStaticLOD(
            distanceToCamera, forceUpdate, manager);
        LS_LOADING_STATUS secondStatus = loadedTree->SecondChild.UpdateStaticLOD(
            distanceToCamera, forceUpdate, manager);
        if (firstStatus == LS_LOADING_FAILED || secondStatus == LS_LOADING_FAILED)
            return LS_LOADING_FAILED;
        if (firstStatus == LS_LOADING_PENDING || secondStatus == LS_LOADING_PENDING)
            return LS_LOADING_PENDING;
        return LS_LOADING_READY;
    }

    bool culled = true;
    const float* plane = reinterpret_cast<const float*>(DAT_01436ea0 + 0x1C0);
    for (int planeIndex = 0; planeIndex < 4; ++planeIndex, plane += 4)
    {
        float x = plane[0] <= DAT_0122dedc ? BoundsMaxX : BoundsMinX;
        float y = plane[1] <= DAT_0122dedc ? BoundsMaxY : BoundsMinY;
        float z = plane[2] <= DAT_0122dedc ? BoundsMaxZ : BoundsMinZ;
        if (plane[3] < x * plane[0] + y * plane[1] + z * plane[2])
        {
            culled = false;
            break;
        }
    }

    bool started;
    if (CurrentLODLevel < 8)
    {
        unsigned long detailLevel =
            StaticLODEntries[RequestedLODEntryIndex].PatchDetailLevel;
        started = StartPatchLoadTask(detailLevel, manager, !culled);
    }
    else
    {
        started = StartTreeLoadTask(manager, !culled);
    }
    if (started)
        return ProcessLoadTask(foregroundRange, distanceToCamera, forceUpdate, manager);
    return LS_LOADING_FAILED;
}
