#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "fable_counted_pointer.h"
#include "engine/CEngineResourceManager.h"
CMovableResourceMemoryPool* CEngine::GetLoadingMemoryPool()
{
    CCountedPointer<CMovableResourceMemoryPool> pool;
    pool.Info=NEngineInternal::GPResourceManager->LoadingMemoryPool.Info;
    pool.Data=NEngineInternal::GPResourceManager->LoadingMemoryPool.Data;
    if(pool.Info)++pool.Info->RefCount;
    return pool.Get();
}
CMovableResourceMemoryPool* CEngine::GetAnimationMemoryPool()
{
    CCountedPointer<CMovableResourceMemoryPool> pool;
    pool.Info=NEngineInternal::GPResourceManager->AnimationMemoryPool_Info;
    pool.Data=NEngineInternal::GPResourceManager->AnimationMemoryPool_Data;
    if(pool.Info)++pool.Info->RefCount;
    return pool.Get();
}
CGPUScratchBufferTextureManager* CEngine::GetScratchBufferTextureManager()
{
    return reinterpret_cast<CGPUScratchBufferTextureManager*>(NEngineInternal::GPResourceManager->GPUScratchBufferTextureManager);
}
