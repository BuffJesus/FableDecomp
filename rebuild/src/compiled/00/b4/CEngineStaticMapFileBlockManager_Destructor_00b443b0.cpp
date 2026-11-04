#include "engine/CEngineStaticMapFileBlockManager.h"
namespace _STL {
template<> inline map<long,CEngineStaticMapFileBlockManager::CFileBlock>::~map() {
    _M_t.clear();
    if(_M_t._M_header._M_data)
        _M_t._M_header.deallocate(_M_t._M_header._M_data,1);
}
}
CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager() {}
