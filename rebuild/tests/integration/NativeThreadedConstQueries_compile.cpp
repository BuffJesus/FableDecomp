// Compile-time const contract for the reduced STLport UID map.
#include "fable_threaded_file_object.h"
typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*> QueryMap;
typedef QueryMap::iterator QueryIterator;
typedef QueryMap::const_iterator QueryConstIterator;
FABLE_STATIC_ASSERT(sizeof(QueryConstIterator)==4);

#if defined(FABLE_NEGATIVE_CONST_WRITE)
void RejectConstWrite(const QueryMap& tree) { tree.begin()->second=0; }
#elif defined(FABLE_NEGATIVE_CONST_CONVERSION)
QueryIterator RejectConstConversion(const QueryMap& tree) { return tree.begin(); }
#else
bool CheckMixedIteratorComparison(QueryMap& tree,const QueryMap& view)
{
    QueryIterator it=tree.begin();
    QueryConstIterator converted=it;
    return converted==it && it==converted && !(converted!=it) && !(it!=converted)
        && view.find(17)==view.end();
}
void CheckConstPointerPayload(const QueryMap& view)
{
    QueryConstIterator it=view.begin();
    if(it!=view.end() && it->second)
        it->second->LoadingStatus=CThreadedFileManager::FILE_LOADING_STATUS_FINISHED;
}
#endif
