// STLport _Rb_tree<long, pair<const long, CCharString>, ...>::clear() @ 009f9a50 (51 bytes)
//
// The tree behind CProfileManager::IDToProfileName (Ego_r PDB:
// map<long,CCharString> at CProfileManager+0x50 in retail). Its out-of-line
// _M_erase is 0x00451B4B: ~CProfileManager (0x009F9020) calls exactly that
// _M_erase for the +0x50 map (0x009F9053 / 0x009F90EE), and the +0x5C
// map<CCharString,long> uses its own _M_erase 0x0043020A. This clear() calls
// 0x00451B4B, so it is the map<long,CCharString> tree's clear. The body sits in
// the CProfileManager object's COMDAT run (0x009F9450 CProfile copy ctor,
// 0x009F9930 Children vector copy ctor, 0x009F9A90 tree-entry _Cons_val).
// No retail caller (the dtor inlines it); the instantiation survives.
// The manifest's "CTCCarryable::OnKill" is a propagated label.

#include "fable_stlport_tree.h"

template void _STL::_Rb_tree<
    long,
    _STL::pair<const long, CCharString>,
    _STL::_Select1st<_STL::pair<const long, CCharString> >,
    _STL::less<long>,
    _STL::allocator<_STL::pair<const long, CCharString> > >::clear();
