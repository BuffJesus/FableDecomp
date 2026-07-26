#include <new>
#include "candidates/landscape_background_lod_compiled.h"

LS_LOADING_STATUS CEngineLandscapeMap::UpdateBackgroundLOD(
    CLandscapeGPUUpdateQueue&,
    double distanceToCamera,
    bool forceUpdate)
{
    LS_LOADING_STATUS status = LS_LOADING_READY;
    LandscapeUpdateActive = true;

    if (BackgroundTree == 0)
    {
        if (BackgroundLoadEntry != 0)
        {
            if (!BackgroundHeaderRequestIssued)
            {
                StaticMapFileBlockManager->AddRequest(
                    BackgroundHeaderBlockId, BackgroundHeaderBlockSize, false);
                BackgroundHeaderRequestIssued = true;
            }
            if (StaticMapFileBlockManager->IsBlockLoaded(BackgroundHeaderBlockId))
            {
                void* storage = AllocateLandscapeBackgroundTreeNode(0xE8, this);
                BackgroundTree = storage != 0 ?
                    new(storage) CLandscapeBackgroundTreeNode(this, 0, LandscapeMapIndex) : 0;
                if (BackgroundTree != 0)
                {
                    unsigned char* blockData = StaticMapFileBlockManager->GetBlockData(
                        BackgroundHeaderBlockId, BackgroundHeaderBlockSize);
                    fable_u8 streamStorage[0x1C];
                    new(streamStorage + 4) CMemoryDataInputStream(blockData);
                    CMemoryDataInputStream* stream =
                        reinterpret_cast<CMemoryDataInputStream*>(streamStorage);
                    BackgroundTree->LoadHeader(*stream);
                    ReleaseFileBlockLoad();
                    stream->~CMemoryDataInputStream();
                }
            }
            else
            {
                status = LS_LOADING_PENDING;
            }
        }
        else if (OwnerMap->EdgeHeightFile == 0)
        {
            void* storage = AllocateLandscapeBackgroundTreeNode(0xE8, this);
            BackgroundTree = storage != 0 ?
                new(storage) CLandscapeBackgroundTreeNode(this, 0, LandscapeMapIndex) : 0;
            BackgroundTree->BuildDefaultSection(
                0, 0, OwnerMap->BackgroundWidth, OwnerMap->BackgroundHeight);
        }
    }

    if (BackgroundTree != 0 && !ForegroundLODUpdateActive)
        status = BackgroundTree->UpdateStaticLOD(
            distanceToCamera, forceUpdate, StaticMapFileBlockManager);

    if (StaticMapFileBlockManager != 0)
        StaticMapFileBlockManager->BankFileList_RemoveFinished();
    LandscapeUpdateActive = false;
    return status;
}
