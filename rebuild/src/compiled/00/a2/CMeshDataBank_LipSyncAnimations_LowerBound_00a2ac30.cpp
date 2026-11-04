// _Rb_tree<CCharString, pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >::_M_lower_bound,
// retail 0x00A2AC30: the tree behind CMeshDataBank::LipSyncAnimations (Ego_r map<CCharString,
// CArray<CMeshDataBank::CLipSyncEntry> >, retail +0x3C8). Callers: the map's operator[] 0x00A2E290
// (0x00A2E29D; called from 0x00A27BC8 on this+0x3C8) and its lower_bound wrappers 0x00A2B920 /
// 0x00A2BFA0. CCharString's operator< is expanded and calls CBasicString<char>::operator<
// 0x00429950 (as in the CConsole tree 0x009EEC10). FableWin names the same map:
// ?lower_bound@?$_Tree@V?$_Tmap_traits@VCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@...
#include "engine/CMeshDataBank.h"
#include "fable_stlport_tree_bounds.h"

typedef _STL::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> >::_Rep_type CLipSyncAnimationTree;
template CLipSyncAnimationTree::_Link_type CLipSyncAnimationTree::_M_lower_bound(const CCharString&) const;
