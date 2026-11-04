"""Fresh VC71 parity gate: STLport vector<CVertexBufferWin32*>::_M_fill_insert (CFontManager vertex-buffer table growth; memmove shifts, fastcall fill helper)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$fill@PAPAVCVertexBufferWin32@@PAV1@@_STL@@YIXPAPAVCVertexBufferWin32@@0ABQAV1@@Z':0x9e2a80,
 '_memmove':0xbfeae6,'_malloc':0xbfea0e,'_free':0xbfea14,
 '?_M_insert_overflow@?$vector@PAVCVertexBufferWin32@@V?$allocator@PAVCVertexBufferWin32@@@_STL@@@_STL@@QAEXPAPAVCVertexBufferWin32@@ABQAV3@ABU__true_type@2@I_N@Z':0x9e3330,
}
F=R/'rebuild/src/compiled/00/9e/STLFontVertexBufferVector_FillInsert_009e3430.cpp'
ROOTS=[
 dict(name='fill_insert',source=F,symbol='?_M_fill_insert@?$vector@PAVCVertexBufferWin32@@V?$allocator@PAVCVertexBufferWin32@@@_STL@@@_STL@@QAEXPAPAVCVertexBufferWin32@@IABQAV3@@Z',address=0x9e3430,size=276,instructions=110,relocations=6),
 dict(name='insert_overflow',source=F,symbol='?_M_insert_overflow@?$vector@PAVCVertexBufferWin32@@V?$allocator@PAVCVertexBufferWin32@@@_STL@@@_STL@@QAEXPAPAVCVertexBufferWin32@@ABQAV3@ABU__true_type@2@I_N@Z',address=0x9e3330,size=216,instructions=85,relocations=4),
 dict(name='fill',source=F,symbol='??$fill@PAPAVCVertexBufferWin32@@PAV1@@_STL@@YIXPAPAVCVertexBufferWin32@@0ABQAV1@@Z',address=0x9e2a80,size=31,instructions=12,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-font-vertex-buffer-fill-insert.json',[],
  'STLport vector<CVertexBufferWin32*>::_M_fill_insert, _M_insert_overflow and fill (CFontManager vertex-buffer table growth)')
 print('NATIVE_FONT_VERTEX_BUFFER_FILL_INSERT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
