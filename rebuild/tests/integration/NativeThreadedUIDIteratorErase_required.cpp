#include "fable_gfuninitialise.h"
#include "fable_stlport_tree.h"

template<class Map> struct UIDIteratorEraseCaller {
    static void Call(Map* map,typename Map::iterator position)
    {
        map->erase(position);
    }
};

// Private nested name occurs only in the explicit-instantiation declaration.
template struct UIDIteratorEraseCaller<
    _STL::map<unsigned long,CThreadedFileManager::CFileObject*> >;

int main() { return 0; }
