#include "fable_gfuninitialise.h"
#include "fable_stlport_tree_erase.h"

namespace _STL {
template void _Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > >::erase(iterator);
}
