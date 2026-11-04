#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CTextureManager.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "fable_surface_types.h"
#include "fable_engine_internal.h"

bool CEngineResourceManager::ResizeHiresTextureMemory(long memory)
{
    GraphicBanks->ChangeScene();
    NEngineInternal::GPSurfaceCompositionManager->ChangeSceneForTextureBank();
    CPixelFormat format(D3DFMT_DXT1);
    unsigned long size=CTextureManager::CalculateTextureSize(512,512,true,format);
    unsigned long delay=MainGraphicBank->ReduceSizeFrameDelay;
    MainGraphicBank->SetReduceSizeFrameDelay(0);
    long count=static_cast<long>(static_cast<unsigned long>(memory)<<10)/static_cast<long>(size);
    bool result=MainGraphicBank->TextureManager.Data->ResizePoolTextureCount(size,count);
    MainGraphicBank->SetReduceSizeFrameDelay(delay);
    return result;
}
