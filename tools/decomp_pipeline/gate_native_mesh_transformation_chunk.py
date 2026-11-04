"""Fresh VC71 parity gate: C3DMeshFileTransformationChunk info/read/write/create/description (identity transform when new)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z':0x994070,
 '?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ':0x995340,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_04KEIKGGPK@TRFM?$AA@':0x129eaec,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileTransformationChunk@@6B@':0x129eb54,  # derived from retail site; not in symbol_index
 '??_C@_0BG@NNEHMMHB@Transformation?5Matrix?$AA@':0x129eb80,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileTransformationChunk_Methods_00ab9c30.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileTransformationChunk@@UBE?BVCCharString@@XZ',address=0xab9c30,size=25,instructions=9,relocations=2),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileTransformationChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xab9c50,size=91,instructions=38,relocations=2),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileTransformationChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xab9cb0,size=37,instructions=16,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileTransformationChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xab9ce0,size=108,instructions=45,relocations=4),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileTransformationChunk@@UBE?BVCCharString@@XZ',address=0xabd3e0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-transformation-chunk.json',[],
  'C3DMeshFileTransformationChunk info/read/write/create/description (identity transform when new)')
 print('NATIVE_MESH_TRANSFORMATION_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
