#include <new>
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CLocalDetailObjectCollectionType.h"
#include "fable_stlport_vector_impl.h"

const CLocalDetailObjectCollectionType* CEngineLocalDetailGenerator::RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type)
{
    for(size_t i=0;i<ObjectCollectionTypes.size();++i)
        if(ObjectCollectionTypes[i]->IsEquivalent(type)) return ObjectCollectionTypes[i];
    CLocalDetailObjectCollectionType* result=new CLocalDetailObjectCollectionType(type);
    ObjectCollectionTypes.push_back(result);
    return result;
}
