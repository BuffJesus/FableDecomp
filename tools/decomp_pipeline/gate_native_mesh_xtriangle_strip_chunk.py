"""Fresh VC71 parity gate: C3DMeshFileXTriangleStripChunk write/create/read/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@GV?$allocator@G@_STL@@@_STL@@QAEXIABG@Z':0x517900,  # derived from retail site; not in symbol_index
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04MFPMBMKE@XTST?$AA@':0x129df60,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXTriangleStripChunk@@6B@':0x129e380,  # derived from retail site; not in symbol_index
 '??_C@_0P@OOGGGAKM@Triangle?5Strip?$AA@':0x129e3ac,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileXTriangleStripChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa5930,size=69,instructions=33,relocations=1),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileXTriangleStripChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa5a60,size=83,instructions=31,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileXTriangleStripChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa6960,size=246,instructions=102,relocations=3),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileXTriangleStripChunk@@UBE?BVCCharString@@XZ',address=0xaa7280,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-xtriangle-strip-chunk.json',[],
  'C3DMeshFileXTriangleStripChunk write/create/read/description')
 print('NATIVE_MESH_XTRIANGLE_STRIP_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
