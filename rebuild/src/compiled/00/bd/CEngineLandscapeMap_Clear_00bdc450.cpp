#include <new>
#include "candidates/landscape_background_lod_compiled.h"
#include "fable_game_text_bank.h"
#include "fable_counted_pointer_reset_inline.h"
void CEngineLandscapeMap::Clear() {
    delete[] Patches;
    Patches=0;
    delete BackgroundTree;
    BackgroundTree=0;
    MapFileEntry.Reset(0);
    IsDynamic=false;
    if(LoadingPatches.Data)CBankFileAsyncData::DiscardAsyncData(LoadingPatches);
    if(PatchTreeBaseLoading) {
        StaticMapFileBlockManager->ReleaseRequest(BackgroundHeaderBlockId);
        PatchTreeBaseLoading=false;
    }
    PatchesFilePointer=0;
    BackgroundHeaderBlockId=0;
    BackgroundHeaderBlockSize=0;
}
