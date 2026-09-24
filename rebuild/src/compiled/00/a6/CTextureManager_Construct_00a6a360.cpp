#include "fable_ui_texture_manager.h"
void* __fastcall FableUiConstructTextureManager(void* storage,void*)
{
    FableUiTextureManagerView* manager=static_cast<FableUiTextureManagerView*>(storage);
    FableUiConstructBase(manager,0); manager->Vtable=&FableUiTextureManagerVtable;
    manager->Initialised=false; manager->PoolCount=0;
    for(unsigned i=0;i<16;++i)
    {
        FableUiPreallocTexturePool& pool=manager->PreallocPools[i];
        FableUiConstructResourceList(&pool.Resources,0); pool.Resources.__vftable=FableUiPreallocPoolVtable;
        pool.Count=0; pool.MaxSize=0; pool.PoolIndex=-1;
        pool.Textures.Begin=0; pool.Textures.End=0; pool.Textures.Capacity=0;
    }
    manager->BufferSize=0; manager->TextureBuffer=0;
    for(unsigned j=0;j<16;++j) manager->FailedAllocations[j]=0;
    return manager;
}
