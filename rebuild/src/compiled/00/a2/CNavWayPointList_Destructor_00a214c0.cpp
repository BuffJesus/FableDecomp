// _STL::list<CNavWayPoint>::~list (retail 0x00A214C0). Called by ~CNavigatorAStar 0x00A1CB40
// (vtable 0x0129CCCC store, then lea ecx,[esi+8Ch] = CNavigatorAStar::Route); also the target
// of the jmp thunk 0x00A21A00 after the list<CNavWayPoint> ctor 0x00A21980 (0x14-byte node).
// Owner name: STLport ~_List_base, genuine symbol
//   ??1?$_List_base@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@_STL@@@_STL@@QAE@XZ;
// the true ~list is the jmp thunk 0x00A21A00 (unrepresented until fable_stlport_list.h
// models _List_base). Emitted under the list::~list name as in CWallList_clear_00a20e10.cpp.
#include "fable_navigator.h"
#include "fable_stlport_list_impl.h"

template _STL::list<CNavWayPoint>::~list();
