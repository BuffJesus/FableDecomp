#include <new>
#include "engine/CEnginePrimitiveManagerSprites.h"
#include "engine/CEnginePrimitiveTypeList.h"
namespace _STL { template<> void vector<long>::_M_insert_overflow(long*,const long&,const __true_type&,size_t,bool); }
#include "fable_stlport_vector_impl.h"
void CEnginePrimitiveManagerSprites::GetSupportedTypes(CEnginePrimitiveTypeList& types)
{
    types.Add(ENGINE_PRIMITIVE_SPRITE);
}
