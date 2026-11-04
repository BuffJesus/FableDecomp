"""Fresh VC71 parity gate: CVectorMap Sort / Update for the nav quad tree's 64-bit-keyed maps (0x00A84590, 0x00A845E0, 0x00A84B10, 0x00A84B60), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$sort@PAU?$pair@_KPAVCANavQuadTreeNode@@@_STL@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@_STL@@YIXPAU?$pair@_KPAVCANavQuadTreeNode@@@0@0V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@Z':0xa841f0,
 '??$sort@PAU?$pair@_K_N@_STL@@V?$CKeyPairCompareLess@_K_N@@@_STL@@YIXPAU?$pair@_K_N@0@0V?$CKeyPairCompareLess@_K_N@@@Z':0xa84160,
}
ROOTS=[
 dict(name='bool_sort',source=R/'rebuild/src/compiled/00/a8/CVectorMap_NavSortUpdate_00a84590.cpp',symbol='?Sort@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QAEXXZ',address=0xa84590,size=25,instructions=11,relocations=1),
 dict(name='bool_update',source=R/'rebuild/src/compiled/00/a8/CVectorMap_NavSortUpdate_00a84590.cpp',symbol='?Update@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QAEXXZ',address=0xa84b10,size=32,instructions=14,relocations=1),
 dict(name='node_sort',source=R/'rebuild/src/compiled/00/a8/CVectorMap_NavSortUpdate_00a84590.cpp',symbol='?Sort@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEXXZ',address=0xa845e0,size=25,instructions=11,relocations=1),
 dict(name='node_update',source=R/'rebuild/src/compiled/00/a8/CVectorMap_NavSortUpdate_00a84590.cpp',symbol='?Update@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEXXZ',address=0xa84b60,size=32,instructions=14,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-nav-vector-map-sort.json',[],
  "CVectorMap Sort / Update for the nav quad tree's 64-bit-keyed maps (0x00A84590, 0x00A845E0, 0x00A84B10, 0x00A84B60), exa")
 print('NATIVE_NAV_VECTOR_MAP_SORT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
