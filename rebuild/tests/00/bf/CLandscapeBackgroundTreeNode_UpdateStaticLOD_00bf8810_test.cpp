#include <stdio.h>
#include <string.h>
#include "candidates/background_tree_static_lod_compiled.h"

fable_u8* DAT_01436ea0;
float DAT_0122dedc;
static double g_timer;
static bool g_foreground;
static bool g_startPatch, g_startTree;
static LS_LOADING_STATUS g_processStatus;
static int g_processCalls, g_patchCalls, g_treeCalls;
static unsigned long g_detail;
static bool g_immediate;

void CLoadedStaticLODBase::Slot00() {}
void CLoadedStaticLODBase::Slot04() {}
void CLoadedStaticLODBase::Slot08() {}
void CLoadedStaticLODBase::Slot0C() {}
bool CLoadedStaticLODBase::IsPatchObject() { return false; }
bool CLoadedStaticLODPatch::IsPatchObject() { return true; }
bool CLoadedStaticLODTree::IsPatchObject() { return false; }
double GFGetFastSubFrameTimer() { return g_timer; }
bool CLandscapeBackgroundTreeNode::DebugCalcInForgroundRange() { return g_foreground; }

LS_LOADING_STATUS CLandscapeBackgroundTreeNode::ProcessLoadTask(
    int, double, bool, CEngineStaticMapFileBlockManager*)
{
    ++g_processCalls;
    return g_processStatus;
}

bool CLandscapeBackgroundTreeNode::StartPatchLoadTask(
    unsigned long detail, CEngineStaticMapFileBlockManager*, bool immediate)
{
    ++g_patchCalls;
    g_detail = detail;
    g_immediate = immediate;
    return g_startPatch;
}

bool CLandscapeBackgroundTreeNode::StartTreeLoadTask(
    CEngineStaticMapFileBlockManager*, bool immediate)
{
    ++g_treeCalls;
    g_immediate = immediate;
    return g_startTree;
}

static void Reset(CLandscapeBackgroundTreeNode& node)
{
    memset(&node, 0, sizeof(node));
    node.HasStaticLODData = 1;
    node.BoundsMinX = node.BoundsMinY = node.BoundsMinZ = -1.0f;
    node.BoundsMaxX = node.BoundsMaxY = node.BoundsMaxZ = 1.0f;
    g_timer = 0.0;
    g_foreground = false;
    g_startPatch = g_startTree = true;
    g_processStatus = LS_LOADING_READY;
    g_processCalls = g_patchCalls = g_treeCalls = 0;
    g_detail = 0;
    g_immediate = false;
}

int main()
{
    fable_u8 camera[0x200];
    memset(camera, 0, sizeof(camera));
    DAT_01436ea0 = camera;
    DAT_0122dedc = 0.0f;
    CEngineStaticMapFileBlockManager manager;
    CLandscapeBackgroundTreeNode node;

    Reset(node); node.HasStaticLODData = 0;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_READY) return 1;

    Reset(node); g_timer = 20.0;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_PENDING) return 2;

    Reset(node); node.PendingLoadTask = &node; g_processStatus = LS_LOADING_PENDING;
    if (node.UpdateStaticLOD(10.0, true, &manager) != LS_LOADING_PENDING || g_processCalls != 1) return 3;

    Reset(node);
    CLoadedStaticLODPatch patch;
    patch.DetailLevel = 3;
    node.LoadedStaticLOD = &patch;
    node.CurrentLODLevel = 0;
    node.StaticLODEntries[0].PatchDetailLevel = 3;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_READY) return 4;
    if (g_patchCalls != 0 || g_treeCalls != 0) return 5;

    Reset(node); patch.DetailLevel = 4; node.LoadedStaticLOD = &patch;
    node.CurrentLODLevel = 0; node.StaticLODEntries[0].PatchDetailLevel = 3;
    g_processStatus = LS_LOADING_PENDING;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_PENDING) return 6;
    if (g_patchCalls != 1 || g_detail != 3 || g_processCalls != 1) return 7;

    Reset(node); patch.DetailLevel = 4; node.LoadedStaticLOD = &patch;
    node.CurrentLODLevel = 2; g_foreground = false; g_processStatus = LS_LOADING_READY;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_READY) return 8;

    Reset(node);
    CLoadedStaticLODTree tree;
    memset(&tree.FirstChild, 0, sizeof(tree.FirstChild));
    memset(&tree.SecondChild, 0, sizeof(tree.SecondChild));
    node.LoadedStaticLOD = &tree;
    node.CurrentLODLevel = 8;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_READY) return 9;

    Reset(node); node.CurrentLODLevel = 8; g_processStatus = LS_LOADING_FAILED;
    if (node.UpdateStaticLOD(10.0, false, &manager) != LS_LOADING_FAILED) return 10;
    if (g_treeCalls != 1 || g_processCalls != 1) return 11;

    printf("BACKGROUND_TREE_STATIC_LOD_TEST PASS\n");
    return 0;
}
