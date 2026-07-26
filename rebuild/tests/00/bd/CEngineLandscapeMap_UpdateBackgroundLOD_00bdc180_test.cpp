#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/landscape_background_lod_compiled.h"

static int g_addRequests, g_finished, g_constructed, g_defaults, g_headers, g_updates, g_releases;
static bool g_loaded;
static LS_LOADING_STATUS g_updateStatus;
static unsigned long g_width, g_height;
static unsigned char g_blockData[8];

void* __fastcall AllocateLandscapeBackgroundTreeNode(unsigned long bytes, CEngineLandscapeMap*)
{
    return operator new(bytes);
}

void CEngineStaticMapFileBlockManager::AddRequest(long, unsigned long, bool) { ++g_addRequests; }
bool CEngineStaticMapFileBlockManager::IsBlockLoaded(long) { return g_loaded; }
unsigned char* CEngineStaticMapFileBlockManager::GetBlockData(long, unsigned long) { return g_blockData; }
void CEngineStaticMapFileBlockManager::BankFileList_RemoveFinished() { ++g_finished; }
CMemoryDataInputStream::CMemoryDataInputStream(unsigned char*) {}
CMemoryDataInputStream::~CMemoryDataInputStream() {}
CLandscapeBackgroundTreeNode::CLandscapeBackgroundTreeNode(CEngineLandscapeMap*, void*, long) { ++g_constructed; }
void CLandscapeBackgroundTreeNode::BuildDefaultSection(unsigned long, unsigned long, unsigned long width, unsigned long height)
{ ++g_defaults; g_width = width; g_height = height; }
void CLandscapeBackgroundTreeNode::LoadHeader(CMemoryDataInputStream&) { ++g_headers; }
LS_LOADING_STATUS CLandscapeBackgroundTreeNode::UpdateStaticLOD(double, bool, CEngineStaticMapFileBlockManager*)
{ ++g_updates; return g_updateStatus; }
void CEngineLandscapeMap::ReleaseFileBlockLoad() { ++g_releases; }

static void Reset()
{
    g_addRequests = g_finished = g_constructed = g_defaults = g_headers = g_updates = g_releases = 0;
    g_loaded = false;
    g_updateStatus = LS_LOADING_READY;
    g_width = g_height = 0;
}

int main()
{
    CEngineLandscapeMap map;
    CEngineMap owner;
    CEngineStaticMapFileBlockManager manager;
    CLandscapeGPUUpdateQueue queue;
    CLandscapeBackgroundTreeNode existing(&map, 0, 0);

    Reset(); memset(&map, 0, sizeof(map));
    map.StaticMapFileBlockManager = &manager;
    map.BackgroundTree = &existing;
    g_updateStatus = LS_LOADING_PENDING;
    if (map.UpdateBackgroundLOD(queue, 4.0, true) != LS_LOADING_PENDING) return 1;
    if (g_updates != 1 || g_finished != 1 || map.LandscapeUpdateActive) return 2;

    Reset(); memset(&map, 0, sizeof(map)); memset(&owner, 0, sizeof(owner));
    map.OwnerMap = &owner; map.StaticMapFileBlockManager = &manager;
    map.BackgroundLoadEntry = &map; map.BackgroundHeaderBlockId = 7; map.BackgroundHeaderBlockSize = 8;
    if (map.UpdateBackgroundLOD(queue, 1.0, false) != LS_LOADING_PENDING) return 3;
    if (g_addRequests != 1 || !map.BackgroundHeaderRequestIssued || g_finished != 1) return 4;

    Reset(); memset(&map, 0, sizeof(map)); memset(&owner, 0, sizeof(owner));
    owner.BackgroundWidth = 12; owner.BackgroundHeight = 34;
    map.OwnerMap = &owner; map.StaticMapFileBlockManager = &manager;
    if (map.UpdateBackgroundLOD(queue, 2.0, false) != LS_LOADING_READY) return 5;
    if (g_constructed != 1 || g_defaults != 1 || g_width != 12 || g_height != 34 || g_updates != 1) return 6;

    Reset(); memset(&map, 0, sizeof(map)); memset(&owner, 0, sizeof(owner));
    map.OwnerMap = &owner; map.StaticMapFileBlockManager = &manager;
    map.BackgroundLoadEntry = &map; map.BackgroundHeaderBlockId = 9; map.BackgroundHeaderBlockSize = 8;
    g_loaded = true;
    if (map.UpdateBackgroundLOD(queue, 3.0, false) != LS_LOADING_READY) return 7;
    if (g_constructed != 1 || g_headers != 1 || g_releases != 1 || g_updates != 1) return 8;
    printf("LANDSCAPE_BACKGROUND_LOD_TEST PASS\n");
    return 0;
}
