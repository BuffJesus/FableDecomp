"""Fresh VC71 parity gate for CQuickDrawTriInfo vector helpers 009E11A0 and 009E1650.

Both are also emitted as COMDATs by the landed 009E1750 _M_insert_overflow unit; these
dedicated sources explicitly instantiate them so each retail address has one source.
"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CQuickDrawTriInfo@@QAE@ABV0@@Z':0x9de890,
 '?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,
 '??1CWideString@@QAE@XZ':0x99b510,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='uninitialized_copy',source=R/'rebuild/src/compiled/00/9e/STLQuickDrawInfo_UninitializedCopy_009e11a0.cpp',symbol='??$__uninitialized_copy@PAVCQuickDrawTriInfo@@PAV1@@_STL@@YIPAVCQuickDrawTriInfo@@PAV1@00ABU__false_type@0@@Z',address=0x9e11a0,size=46,instructions=23,relocations=1),
 dict(name='clear',source=R/'rebuild/src/compiled/00/9e/STLQuickDrawInfo_Clear_009e1650.cpp',symbol='?_M_clear@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXXZ',address=0x9e1650,size=58,instructions=26,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-quick-draw-info-helpers.json',
  ['work/native-recreation/rendering/009e1750/20261015/native-first (insert-overflow composition executing both helpers)'],
  'Two genuine STLport template instantiations, byte-identical to retail after audited relocation patching.')
 print('NATIVE_QUICK_DRAW_INFO_HELPERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
