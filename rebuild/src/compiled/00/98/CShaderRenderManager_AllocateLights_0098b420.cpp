#include <new>
#include "fable_shader_render_manager.h"
#include "fable_simple_memory_pool.h"
#include "fable_stlport_vector.h"
namespace _STL {
template<> C4DVector* vector<C4DVector>::erase(C4DVector*,C4DVector*);
template<> void vector<C4DVector>::_M_fill_insert(C4DVector*,size_t,const C4DVector&);
}
#include "fable_stlport_vector_impl.h"
// Retail checks the full product before requesting raw array storage.
// Element construction is separate; only newly added light records get defaults.
template<class T> static __forceinline T* AllocatePoolArray(CSimpleMemoryPool& pool,unsigned long count)
{
 return count && (unsigned __int64)count*sizeof(T)<=0xffffffffUL?(T*)pool.Allocate(count*sizeof(T)):0;
}
void CShaderRenderManager::AllocateLights(unsigned long count,unsigned long size)
{
 _STL::vector<CLightInfo> saved(AllocatedLights);
 if(AllocatedLights>0)_STL::copy(Lights,Lights+AllocatedLights,saved.begin());
 unsigned long old=AllocatedLights;
 if(AllocatedLights<count)AllocatedLights=count;
 if(AllocatedLightSize<size)AllocatedLightSize=size;
 CSimpleMemoryPool& pool=LightPool;
 pool.FreeAll();
 Lights=AllocatePoolArray<CLightInfo>(pool,AllocatedLights);
 _STL::copy(saved.begin(),saved.end(),Lights);
 if(count>old)_STL::fill(Lights+old,Lights+count,CLightInfo());
 LightBuffer=AllocatePoolArray<C4DVector>(pool,AllocatedLightSize*AllocatedLights);
 C4DVector ones;ones.X=1.0f;ones.Y=1.0f;ones.Z=1.0f;ones.W=1.0f;
 LightAttenuationVector().resize(2*((count+3)>>2),ones);
}
