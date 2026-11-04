// STLport _Rb_tree<unsigned long, pair<const unsigned long, CCountedPointer<C3DAnimation2> >, ...>::clear()
// @ 0x00A2C0B0 (51 bytes).
//
// The tree behind C3DAnimationInfo::CAnimationData::Animations (Ego_r.pdb:
// map<unsigned long, CCountedPointer<C3DAnimation2> > Animations). This clear()
// calls the recursive node erase 0x00A2B9D0, which ~CAnimationData (0x00A26860,
// slot-0 target of ??_GCAnimationData@C3DAnimationInfo@@ 0x00A2DB10) calls at
// 0x00A268D0 for the same tree; the inlined node destruction is exactly
// ~CCountedPointer at node+0x14. No retail caller (the instantiation survives
// unreferenced). The manifest label "CTCCarryable::OnKill" is propagated.
#include "fable_stlport_tree.h"
#include "fable_counted_pointer.h"

class C3DAnimation2;

template void _STL::_Rb_tree<
    unsigned long,
    _STL::pair<const unsigned long, CCountedPointer<C3DAnimation2> >,
    _STL::_Select1st<_STL::pair<const unsigned long, CCountedPointer<C3DAnimation2> > >,
    _STL::less<unsigned long>,
    _STL::allocator<_STL::pair<const unsigned long, CCountedPointer<C3DAnimation2> > > >::clear();
