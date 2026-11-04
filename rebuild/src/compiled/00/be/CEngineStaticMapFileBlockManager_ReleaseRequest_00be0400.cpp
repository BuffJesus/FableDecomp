#include "engine/CEngineStaticMapFileBlockManager.h"
void CEngineStaticMapFileBlockManager::ReleaseRequest(long id) {
    --Blocks.find(id)->second.RefCount;
}
