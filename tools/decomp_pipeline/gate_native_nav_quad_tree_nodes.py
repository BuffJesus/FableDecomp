"""Fresh VC71 parity gate: Concrete navigation quad-tree node overrides (CNavQuadTreeNode, CNavBlockedQuadTreeNode, CNavNavigableLeafNode, CNavSwitchableLeafNode; retail inline-virtual COMDATs 0xA75B40..0xA84E30). Hand-owned fable_nav_quad_tree_nodes.h, ENavQuadTreeNodeType enumerators from Ego_r; journal 2026-10-29."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='cnavquadtreenode_hasneighbours',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?HasNeighbours@CNavQuadTreeNode@@UBE_NXZ',address=0xa7b930,size=3,instructions=2,relocations=0),
 dict(name='cnavquadtreenode_getregionid',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetRegionID@CNavQuadTreeNode@@UBEJXZ',address=0xa7b940,size=3,instructions=2,relocations=0),
 dict(name='cnavquadtreenode_getnodetype',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNodeType@CNavQuadTreeNode@@UBE?AW4ENavQuadTreeNodeType@@XZ',address=0xa7b950,size=3,instructions=2,relocations=0),
 dict(name='cnavblocked_hasneighbours',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?HasNeighbours@CNavBlockedQuadTreeNode@@UBE_NXZ',address=0xa7d920,size=3,instructions=2,relocations=0),
 dict(name='cnavblocked_getregionid',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetRegionID@CNavBlockedQuadTreeNode@@UBEJXZ',address=0xa7d930,size=3,instructions=2,relocations=0),
 dict(name='cnavblocked_getnodetype',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNodeType@CNavBlockedQuadTreeNode@@UBE?AW4ENavQuadTreeNodeType@@XZ',address=0xa7d940,size=6,instructions=2,relocations=0),
 dict(name='cnavblocked_getleafnodeat',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetLeafNodeAt@CNavBlockedQuadTreeNode@@MBEPBVCANavQuadTreeNode@@ABVC2DVector@@@Z',address=0xa76270,size=5,instructions=2,relocations=0),
 dict(name='cnavblocked_makeregion',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?MakeRegion@CNavBlockedQuadTreeNode@@MAE_NPBVCNavRegion@@AAV?$list@PAVCANavQuadTreeNode@@V?$allocator@PAVCANavQuadTreeNode@@@_STL@@@_STL@@@Z',address=0xa7d950,size=5,instructions=2,relocations=0),
 dict(name='cnavblocked_addnavigable',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?AddNavigableNodeNeighbouringSwitchableNodeToRegion@CNavBlockedQuadTreeNode@@MAE_NPBVCNavRegion@@AAV?$list@PAVCANavQuadTreeNode@@V?$allocator@PAVCANavQuadTreeNode@@@_STL@@@_STL@@@Z',address=0xa7d960,size=5,instructions=2,relocations=0),
 dict(name='cnavblocked_removeregionzero',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?RemoveRegionZero@CNavBlockedQuadTreeNode@@MAE_NXZ',address=0xa7d970,size=3,instructions=2,relocations=0),
 dict(name='cnavblocked_getmemoryusage',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetMemoryUsage@CNavBlockedQuadTreeNode@@MBEKAAK000@Z',address=0xa76290,size=5,instructions=2,relocations=0),
 dict(name='cnavblocked_addtoindextable',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?AddToIndexTable@CNavBlockedQuadTreeNode@@MBEXAAJAAV?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@_STL@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@_STL@@@3@@_STL@@@Z',address=0xa76280,size=9,instructions=3,relocations=0),
 dict(name='cnavblocked_convertindexestopointers',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?ConvertIndexesToPointers@CNavBlockedQuadTreeNode@@MAEXABV?$map@JPAVCANavQuadTreeNode@@U?$less@J@_STL@@V?$allocator@U?$pair@$$CBJPAVCANavQuadTreeNode@@@_STL@@@3@@_STL@@@Z',address=0xa7d980,size=3,instructions=1,relocations=0),
 dict(name='cnavnavigable_getneighbours',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNeighbours@CNavNavigableLeafNode@@UAEPAV?$CSmallVector@PBVCANavQuadTreeNode@@$07@@XZ',address=0xa7f2a0,size=4,instructions=2,relocations=0),
 dict(name='cnavnavigable_getneighbours_const',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNeighbours@CNavNavigableLeafNode@@UBEPBV?$CSmallVector@PBVCANavQuadTreeNode@@$07@@XZ',address=0xa7f290,size=4,instructions=2,relocations=0),
 dict(name='cnavnavigable_getnodetype',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNodeType@CNavNavigableLeafNode@@UBE?AW4ENavQuadTreeNodeType@@XZ',address=0xa7f2b0,size=6,instructions=2,relocations=0),
 dict(name='cnavnavigable_getleafnodeat',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetLeafNodeAt@CNavNavigableLeafNode@@MBEPBVCANavQuadTreeNode@@ABVC2DVector@@@Z',address=0xa75b40,size=5,instructions=2,relocations=0),
 dict(name='cnavnavigable_removeregionzero',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?RemoveRegionZero@CNavNavigableLeafNode@@MAE_NXZ',address=0xa75b50,size=11,instructions=6,relocations=0),
 dict(name='cnavswitchable_hasneighbours',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?HasNeighbours@CNavSwitchableLeafNode@@UBE_NXZ',address=0xa76530,size=21,instructions=10,relocations=0),
 dict(name='cnavswitchable_getregionid',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetRegionID@CNavSwitchableLeafNode@@UBEJXZ',address=0xa75be0,size=20,instructions=10,relocations=0),
 dict(name='cnavswitchable_getregion',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetRegion@CNavSwitchableLeafNode@@UBEPBVCNavRegion@@XZ',address=0xa75c00,size=14,instructions=7,relocations=0),
 dict(name='cnavswitchable_getpreferabilitycost',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetPreferabilityCost@CNavSwitchableLeafNode@@UBEEXZ',address=0xa75c10,size=14,instructions=7,relocations=0),
 dict(name='cnavswitchable_getnodetype',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetNodeType@CNavSwitchableLeafNode@@UBE?AW4ENavQuadTreeNodeType@@XZ',address=0xa84e00,size=6,instructions=2,relocations=0),
 dict(name='cnavswitchable_isswitchable',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?IsSwitchable@CNavSwitchableLeafNode@@UBE_NXZ',address=0xa84e10,size=3,instructions=2,relocations=0),
 dict(name='cnavswitchable_addnavigable',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?AddNavigableNodeNeighbouringSwitchableNodeToRegion@CNavSwitchableLeafNode@@MAE_NPBVCNavRegion@@AAV?$list@PAVCANavQuadTreeNode@@V?$allocator@PAVCANavQuadTreeNode@@@_STL@@@_STL@@@Z',address=0xa84e20,size=5,instructions=2,relocations=0),
 dict(name='cnavswitchable_removeregionzero',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?RemoveRegionZero@CNavSwitchableLeafNode@@MAE_NXZ',address=0xa84e30,size=3,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-nav-quad-tree-nodes.json',[],
  'Concrete navigation quad-tree node overrides (CNavQuadTreeNode, CNavBlockedQuadTreeNode, CNavNavigableLeafNode, CNavSwit')
 print('NATIVE_NAV_QUAD_TREE_NODES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
