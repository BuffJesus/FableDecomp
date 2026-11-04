// Candidate-bound fixture for the two _STL::list<CWall> roots of CWallList_clear_00a20e10.cpp
// (one landed object defines both out of line; genuine owners _List_base<CWall>::clear /
// ~_List_base<CWall>, see the source):
//   clear 0x00A20E10: frees every node, keeps the sentinel and leaves it self-linked;
//   ~list 0x00A21250: destroying a populated list (as ??_GCSBCCorridor 0x00A22330 does for
//                     Walls) or an empty one frees every node and the sentinel.
// Heap blocks are counted with _heapwalk before any stdio call.
#include "fable_steering_behaviour.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
static long UsedBlocks()
{
    _HEAPINFO info; info._pentry = 0; long used = 0;
    while (_heapwalk(&info) == _HEAPOK) if (info._useflag == _USEDENTRY) ++used;
    return used;
}
typedef _STL::list<CWall> TestList;
static TestList::_Node* Append(TestList& list, float x)
{
    TestList::_Node* node = static_cast<TestList::_Node*>(malloc(sizeof(TestList::_Node)));
    node->_M_data.Point1 = C2DVector(x, x); node->_M_data.Point2 = C2DVector(x, -x); node->_M_data.ToInternal = C2DVector(0.0f, 1.0f);
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
        TestList walls;
        Append(walls, 1.0f); Append(walls, 2.0f); Append(walls, 3.0f);
        ok = ok && UsedBlocks() == baseline + 4 && walls.size() == 3;
        walls.clear();
        ok = ok && walls.empty() && walls._M_node->_M_prev == walls._M_node && UsedBlocks() == baseline + 1;
        walls.clear();
        ok = ok && walls.empty() && UsedBlocks() == baseline + 1;
        Append(walls, 4.0f);
        ok = ok && walls.size() == 1 && walls.front().Point2.Y == -4.0f;
    }
    ok = ok && UsedBlocks() == baseline;
    {
        TestList walls;
        Append(walls, 5.0f); Append(walls, 6.0f);
        ok = ok && UsedBlocks() == baseline + 3;
    }
    ok = ok && UsedBlocks() == baseline;
    {
        TestList empty;
        ok = ok && UsedBlocks() == baseline + 1;
    }
    ok = ok && UsedBlocks() == baseline;
    if (ok) { printf("PASS_00a20e10_CWALL_LIST_CLEAR_DTOR PASS\n"); return 0; }
    printf("PASS_00a20e10_CWALL_LIST_CLEAR_DTOR FAIL\n");
    return 1;
}
