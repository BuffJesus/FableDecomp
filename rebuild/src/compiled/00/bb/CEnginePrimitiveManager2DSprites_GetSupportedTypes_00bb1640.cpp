#include <new>
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEnginePrimitiveTypeList.h"
namespace _STL { template<> void vector<long>::_M_insert_overflow(long*,const long&,const __true_type&,size_t,bool); }
#include "fable_stlport_vector_impl.h"
void CEnginePrimitiveManager2DSprites::GetSupportedTypes(CEnginePrimitiveTypeList& types)
{
    types.Add(ENGINE_PRIMITIVE_2D_SPRITE);
    types.Add(ENGINE_PRIMITIVE_2D_CLOCK_SPRITE);
}
