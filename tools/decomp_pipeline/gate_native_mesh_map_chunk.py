"""Fresh VC71 parity gate: C3DMeshFileMapChunk info/write/create/read/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04DLCDKOFJ@MMAP?$AA@':0x129edd4,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileMapChunk@@6B@':0x129ee34,  # derived from retail site; not in symbol_index
 '??_C@_0M@NLOOICDG@Texture?5Map?$AA@':0x129ee60,  # derived from retail site; not in symbol_index
 '??_C@_0O@BKHMEJIK@Description?3?5?$AA@':0x129ee6c,  # derived from retail site; not in symbol_index
 '??_C@_0N@DJCOKLJM@?0?5Filename?3?5?$AA@':0x129ee7c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileMapChunk_Methods_00abfe80.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileMapChunk@@UBE?BVCCharString@@XZ',address=0xabfe80,size=153,instructions=44,relocations=13),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileMapChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabff70,size=59,instructions=24,relocations=4),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileMapChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabffb0,size=85,instructions=33,relocations=6),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileMapChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xac03f0,size=200,instructions=78,relocations=6),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileMapChunk@@UBE?BVCCharString@@XZ',address=0xac1290,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-map-chunk.json',[],
  'C3DMeshFileMapChunk info/write/create/read/description')
 print('NATIVE_MESH_MAP_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
