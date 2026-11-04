#include "engine/CPreallocTexturePool.h"
#include "fable_managed_texture_data.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> inline CIVCountedPointer<CManagedTextureData>* vector<CIVCountedPointer<CManagedTextureData> >::erase(CIVCountedPointer<CManagedTextureData>* first,CIVCountedPointer<CManagedTextureData>* last){
    CIVCountedPointer<CManagedTextureData>* result=copy(last,_M_finish,first);
    _Destroy(result,_M_finish);
    _M_finish=result;
    return first;
}
}
template<> void CArray<CIVCountedPointer<CManagedTextureData> >::DeallocateMemory(){
    if(capacity()){
        clear();
        _STL::vector<CIVCountedPointer<CManagedTextureData> > empty;
        empty.swap(*this);
    }
}
