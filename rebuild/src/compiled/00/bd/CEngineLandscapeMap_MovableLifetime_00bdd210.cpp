#include "candidates/landscape_background_lod_compiled.h"

CEngineLandscapeMap::~CEngineLandscapeMap()
{
    if(PatchTreeBaseLoading) {
        StaticMapFileBlockManager->ReleaseRequest(BackgroundHeaderBlockId);
        PatchTreeBaseLoading=false;
    }
    delete[] Patches;
    delete BackgroundTree;
}

bool CEngineLandscapeMap::CanRelocateData(long) const
{
    return !MemoryFreeingLocked;
}
