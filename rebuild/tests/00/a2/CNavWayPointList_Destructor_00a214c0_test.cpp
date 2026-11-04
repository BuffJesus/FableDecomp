// Candidate-bound fixture for _STL::list<CNavWayPoint>::~list (retail 0x00A214C0; genuine owner
// ~_List_base<CNavWayPoint>; the true ~list is the jmp thunk 0x00A21A00): links against the landed
// object of CNavWayPointList_Destructor_00a214c0.cpp and checks that destroying a populated route
// (as ~CNavigatorAStar 0x00A1CB40 does for Route) frees every 0x14-byte node and the sentinel.
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
    bool ok = sizeof(TestList::_Node) == 0x14;
    {
        TestList route;
        Append(route, 1.0f); Append(route, 2.0f); Append(route, 3.0f);
        ok = ok && UsedBlocks() == baseline + 4 && route.size() == 3;
    }
    ok = ok && UsedBlocks() == baseline;
    if (ok) { printf("PASS_00a214c0_NAVWAYPOINT_LIST_DTOR PASS\n"); return 0; }
    printf("PASS_00a214c0_NAVWAYPOINT_LIST_DTOR FAIL\n");
    return 1;
}
