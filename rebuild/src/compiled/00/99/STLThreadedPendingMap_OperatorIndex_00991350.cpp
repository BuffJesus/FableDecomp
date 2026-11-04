// Pending request map lookup-or-insert00991350;124B near98%.
#include "fable_threaded_file_object.h"
#include "fable_stlport_tree_bounds.h"
#include "fable_stlport_tree_impl.h"
namespace _STL {
template CThreadedFileManager::CFileObject*& map<CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*>::operator[](const CThreadedFileManager::CPendingObjectSortKey&);
}
