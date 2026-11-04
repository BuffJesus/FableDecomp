"""Fresh VC71 parity gate: C3DAnimFileMovementVectorChunk info/write/read/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_04NKNNJHIJ@MVEC?$AA@':0x129e7a4,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileMovementVectorChunk@@6B@':0x129e83c,  # derived from retail site; not in symbol_index
 '??_C@_0BA@FDKPGLEC@Movement?5Vector?$AA@':0x129e868,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DAnimFileMovementVectorChunk_Methods_00aae0f0.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DAnimFileMovementVectorChunk@@UBE?BVCCharString@@XZ',address=0xaae0f0,size=25,instructions=9,relocations=2),
 dict(name='write',source=F,symbol='?WriteToFile@C3DAnimFileMovementVectorChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaae130,size=59,instructions=24,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DAnimFileMovementVectorChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaae2c0,size=310,instructions=120,relocations=4),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DAnimFileMovementVectorChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaae400,size=62,instructions=28,relocations=4),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DAnimFileMovementVectorChunk@@UBE?BVCCharString@@XZ',address=0xaaee90,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-movement-vector-chunk.json',[],
  'C3DAnimFileMovementVectorChunk info/write/read/create/description')
 print('NATIVE_ANIM_MOVEMENT_VECTOR_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
