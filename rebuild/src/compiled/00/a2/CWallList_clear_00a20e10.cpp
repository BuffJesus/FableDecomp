// _STL::list<CWall> out-of-line members of the retail steering TU (one source, both roots):
//   clear  0x00A20E10  only caller CSBCCorridor::Process 0x00A880C0 (this in ebx;
//                      lea esi,[ebx+18h] = CSBCCorridor::Walls, Ego_r list<CWall>).
//   ~list  0x00A21250  called by ??_GCSBCCorridor 0x00A22330 (CSBCCorridor vtable 0x0129CCB0
//                      slot 0: lea ecx,[esi+18h] = Walls, then the CSB vtable store 0x0129CC1C);
//                      also the target of the jmp thunk 0x00A21850 after the list<CWall> ctor
//                      0x00A21830 (0x20-byte node).
// Owner names: in real STLport 4.x these bodies are _List_base members (retail pairs each
// list ctor with a jmp thunk to the full body: 0x00A21850 `jmp 0x00A21250`), so the genuine
// symbols are
//   0x00A20E10 ?clear@?$_List_base@VCWall@@V?$allocator@VCWall@@@_STL@@@_STL@@QAEXXZ
//   0x00A21250 ??1?$_List_base@VCWall@@V?$allocator@VCWall@@@_STL@@@_STL@@QAE@XZ
//   0x00A21850 ??1?$list@VCWall@@V?$allocator@VCWall@@@_STL@@@_STL@@QAE@XZ (thunk, unrepresented)
// fable_stlport_list.h models a single-class list without _List_base, so these roots are
// emitted under the list::clear / list::~list names, as the accepted list<CIEngine*>::~list
// 0x00436900 provider does (fable_stlport_list_impl.h). Project-wide gap: GOTCHAS/PLAN.
#include "fable_steering_behaviour.h"
#include "fable_stlport_list_impl.h"

template void _STL::list<CWall>::clear();
template _STL::list<CWall>::~list();
