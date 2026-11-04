#include <new>
#include "candidates/landscape_background_lod_compiled.h"
#include "fable_game_text_bank.h"
void CEngineLandscapeMap::OnMapVisibilityChanged(bool visible) {
    if(!visible) {
        if(LoadingPatches.Data)CBankFileAsyncData::DiscardAsyncData(LoadingPatches);
        if(!IsDynamic) {
            delete[] Patches;
            Patches=0;
            if(PatchTreeBaseLoading) {
                StaticMapFileBlockManager->ReleaseRequest(BackgroundHeaderBlockId);
                PatchTreeBaseLoading=false;
            }
            delete BackgroundTree;
            BackgroundTree=0;
        }
        if(StaticMapFileBlockManager)StaticMapFileBlockManager->DeleteUnusedBlocks();
    }
}
