// _STL::list<CNavWayPoint>::pop_front (retail 0x00A219E0). Only caller 0x00A1FA4F (A* navigator
// code) runs it on CNavigatorAStar::Route (lea ecx,[esi+8Ch]). The inline STLport member of
// fable_stlport_list.h stays out of line as a COMDAT.
#include "fable_navigator.h"

template void _STL::list<CNavWayPoint>::pop_front();
