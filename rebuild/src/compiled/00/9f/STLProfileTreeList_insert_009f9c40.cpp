#include "fable_system_manager.h"
namespace _STL {
template<class T,class A> typename list<T,A>::iterator list<T,A>::insert(iterator position) {
    return insert(position,T());
}
template list<CProfileManager::CProfileTreeEntry>::iterator list<CProfileManager::CProfileTreeEntry>::insert(iterator);
}