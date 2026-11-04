#include "fable_threaded_file.h"
#include "fable_threaded_file_object.h"
// Retail 0x0098F9D0: the pending queue iterator's operator++ (out of line, _M_increment inlined).
namespace _STL {
template _Rb_tree_iterator<pair<const CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*> >&
    _Rb_tree_iterator<pair<const CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*> >::operator++();
}
