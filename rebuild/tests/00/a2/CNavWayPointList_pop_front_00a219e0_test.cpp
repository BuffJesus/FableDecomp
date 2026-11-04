// Fixture for _STL::list<CNavWayPoint>::pop_front (retail 0x00A219E0). pop_front is an inline
// member of fable_stlport_list.h, so this /Od test TU also emits a COMDAT copy; the candidate
// object is linked first, so the linker keeps the landed COMDAT from
// CNavWayPointList_pop_front_00a219e0.cpp. Checks that the front node is unhooked and freed and
// that the remaining route keeps its order (CNavigatorAStar::Route, caller 0x00A1FA4F).
#include <new>
#include "fable_nav_waypoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
static long UsedBlocks()
{
    _HEAPINFO info; info._pentry = 0; long used = 0;
    while (_heapwalk(&info) == _HEAPOK) if (info._useflag == _USEDENTRY) ++used;
    return used;
}
typedef _STL::list<CNavWayPoint> TestList;
static TestList::_Node* Append(TestList& list, float x)
{
    TestList::_Node* node = static_cast<TestList::_Node*>(malloc(sizeof(TestList::_Node)));
    node->_M_data.Position = C2DVector(x, x); node->_M_data.NavLayer = 1; node->_M_data.Removable = true;
    _STL::_List_node_base* sentinel = list._M_node;
    node->_M_next = sentinel; node->_M_prev = sentinel->_M_prev;
    sentinel->_M_prev->_M_next = node; sentinel->_M_prev = node;
    return node;
}
int main()
{
    const long baseline = UsedBlocks();
    bool ok = true;
    {
        // placement-constructed: this object has no ~list (only pop_front is linked in)
        static double raw[(sizeof(TestList) + 7) / 8];
        TestList& route = *new (raw) TestList;
        Append(route, 1.0f); TestList::_Node* second = Append(route, 2.0f); Append(route, 3.0f);
        route.pop_front();
        ok = ok && UsedBlocks() == baseline + 3 && route.size() == 2 && route._M_node->_M_next == second
                && second->_M_prev == route._M_node && route.front().Position.X == 2.0f;
        route.pop_front(); route.pop_front();
        ok = ok && route.empty() && route._M_node->_M_prev == route._M_node && UsedBlocks() == baseline + 1;
        _STL::_List_node_base* sentinel = route._M_node;
        free(sentinel); route._M_node = 0;   // fixture-owned sentinel
    }
    ok = ok && UsedBlocks() == baseline;
    if (ok) { printf("LIST_CNAVWAYPOINT_POP_FRONT PASS\n"); return 0; }
    printf("LIST_CNAVWAYPOINT_POP_FRONT FAIL\n");
    return 1;
}
