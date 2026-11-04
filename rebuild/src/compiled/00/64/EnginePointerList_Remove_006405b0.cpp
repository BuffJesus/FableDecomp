#include "fable_engine_manager.h"
#include "fable_stlport_list_remove_if_impl.h"
namespace _STL {
template<class T,class A> void list<T,A>::remove(const T& value)
{
    iterator first=begin(),last=end();
    while(first!=last){
        iterator next=first;++next;
        if(*first==value)erase(first);
        first=next;
    }
}
template void list<CIEngine*>::remove(CIEngine* const&);
}
