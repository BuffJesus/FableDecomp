#include <new>
#include "engine/CLocalDetailCacheMap.h"
#include "engine/CLocalDetailObjectCollectionType.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "fable_engine_internal.h"
#include "fable_data_input_stream.h"
#include "fable_stlport_vector_impl.h"

void CLocalDetailCacheMap::CObjectTypeCollectionPalette::Load(CDataInputStream& stream)
{
    bool retail=stream.ReadValue<char>()!=0;
    long count=stream.ReadValue<long>();
    Entries.clear();
    for(long i=0;i<count;++i) {
        CLocalDetailObjectCollectionType type;
        type.Load(stream,retail);
        Entries.push_back(NEngineInternal::GPLocalDetailGenerator->RegisterObjectCollectionType(type));
    }
}
