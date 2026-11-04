#include "candidates/landscape_background_lod_compiled.h"
#include "fable_engine_internal.h"
#include "fable_engine_def.h"
#include "engine/CEngineResourceManager.h"
#include "fable_game_text_bank.h"
#include "fable_counted_pointer_reset_inline.h"

CEngineLandscapeMap::CEngineLandscapeMap(const CEngineMap* map,long flags)
    : TexturePalette(NEngineInternal::GPResourceManager->GraphicBanks,false,NEngineInternal::GPEngineDef->InvalidTextureStandin)
{
    CreationFlags=flags;
    OwnerMap=const_cast<CEngineMap*>(map);
    PatchGridWidth=map->MapWidth/16;
    PatchGridHeight=map->MapHeight/16;
    Patches=0;
    BackgroundTree=0;
    PatchTreeBaseLoading=false;
    StaticMapFileBlockManager=0;
    PatchesFilePointer=0;
    BackgroundHeaderBlockId=0;
    BackgroundHeaderBlockSize=0;
    MapFileEntry.Reset(0);
    IsDynamic=false;
    MemoryFreeingLocked=false;
}
