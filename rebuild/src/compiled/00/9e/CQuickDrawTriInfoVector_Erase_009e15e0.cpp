#include <new>
#include "fable_quick_draw_info.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> CQuickDrawTriInfo* vector<CQuickDrawTriInfo>::erase(CQuickDrawTriInfo* first,CQuickDrawTriInfo* last)
{
 CQuickDrawTriInfo* finish=__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
 _Destroy(finish,this->_M_finish);
 this->_M_finish=finish;
 return first;
}
}
