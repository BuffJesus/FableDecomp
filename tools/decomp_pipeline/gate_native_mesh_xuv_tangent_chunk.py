"""Fresh VC71 parity gate: C3DMeshFileXUVTangentChunk write/create/read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@_STL@@@_STL@@QAEXIABVC3DVector@@@Z':0x75c040,  # derived from retail site; not in symbol_index
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04HLILLJCK@XUVT?$AA@':0x129df38,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXUVTangentChunk@@6B@':0x129e3c0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileXUVTangentChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa57f0,size=203,instructions=84,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileXUVTangentChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa5b30,size=85,instructions=38,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileXUVTangentChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa6a60,size=513,instructions=207,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-xuv-tangent-chunk.json',[],
  'C3DMeshFileXUVTangentChunk write/create/read')
 print('NATIVE_MESH_XUV_TANGENT_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
