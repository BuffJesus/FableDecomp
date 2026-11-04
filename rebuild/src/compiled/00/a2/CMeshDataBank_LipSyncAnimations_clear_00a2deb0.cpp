// _Rb_tree<CCharString, pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >::clear(),
// retail 0x00A2DEB0 (51 bytes): the tree behind CMeshDataBank::LipSyncAnimations (retail +0x3C8).
// It calls the recursive node erase 0x00A2DC20, which ~CMeshDataBank 0x00A27350 also calls at
// 0x00A2744A for the +0x3C8 tree (that erase destroys each node's CArray<CLipSyncEntry>, 8-byte
// entries with the CCharString at +0, and its CCharString key). No retail caller: the
// instantiation survives unreferenced, as the C3DAnimationInfo Animations tree clear 0x00A2C0B0
// does. The manifest "CTCCarryable::OnKill" label is propagated.
#include "engine/CMeshDataBank.h"

template void _STL::_Rb_tree<
    CCharString,
    _STL::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> >,
    _STL::_Select1st<_STL::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >,
    _STL::less<CCharString>,
    _STL::allocator<_STL::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > > >::clear();
