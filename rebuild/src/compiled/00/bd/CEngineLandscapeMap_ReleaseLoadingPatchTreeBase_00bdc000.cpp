#include "candidates/landscape_background_lod_compiled.h"
void CEngineLandscapeMap::ReleaseLoadingPatchTreeBase() {
    if(PatchTreeBaseLoading) {
        StaticMapFileBlockManager->ReleaseRequest(BackgroundHeaderBlockId);
        PatchTreeBaseLoading=false;
    }
}
