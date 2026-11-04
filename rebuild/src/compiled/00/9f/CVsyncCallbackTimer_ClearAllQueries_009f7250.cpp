#include "fable_display_viewport.h"
#include "fable_d3d9_query.h"
void CVsyncCallbackTimer::ClearAllQueries()
{
    while(PendingQueories.size()>0) {
        _STL::_List_node<CPendingQuery>* sentinel=PendingQueories._M_node;
        _STL::list<CPendingQuery>::iterator back(sentinel);
        --back;
        IDirect3DQuery9* query=static_cast<_STL::_List_node<CPendingQuery>*>(back._M_node)->_M_data.Query;
        query->Release();
        PendingQueories.pop_back();
    }
    while(FreeQueoryObjects.size()>0) {
        _STL::_List_node<CPendingQuery>* sentinel=FreeQueoryObjects._M_node;
        _STL::list<CPendingQuery>::iterator back(sentinel);
        --back;
        IDirect3DQuery9* query=static_cast<_STL::_List_node<CPendingQuery>*>(back._M_node)->_M_data.Query;
        query->Release();
        FreeQueoryObjects.pop_back();
    }
}
