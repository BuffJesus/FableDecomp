"""Fresh VC71 parity gate: C3DMeshFilePrimitiveChunk write/read/create/description (single or flagged-count material indices)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXIABJ@Z':0x4428f0,  # derived from retail site; not in symbol_index
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04FKFGIHAH@PRIM?$AA@':0x129dee4,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFilePrimitiveChunk@@6B@':0x129ed74,  # derived from retail site; not in symbol_index
 '??_C@_09HDIAEHDD@Primitive?$AA@':0x129eda0,
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFilePrimitiveChunk_Methods_00abc390.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFilePrimitiveChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabc390,size=135,instructions=58,relocations=5),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFilePrimitiveChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xabcf40,size=318,instructions=121,relocations=5),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFilePrimitiveChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabd0d0,size=122,instructions=45,relocations=5),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFilePrimitiveChunk@@UBE?BVCCharString@@XZ',address=0xabee90,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-primitive-chunk.json',[],
  'C3DMeshFilePrimitiveChunk write/read/create/description (single or flagged-count material indices)')
 print('NATIVE_MESH_PRIMITIVE_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
