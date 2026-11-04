"""Fresh VC71 parity gate: C3DMeshFileHelperPointChunk info/write/read/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_02KEGNLNML@?0?5?$AA@':0x12397cc,  # derived from retail site; not in symbol_index
 '??_C@_04DOIACMFC@HPNT?$AA@':0x129e51c,  # derived from retail site; not in symbol_index
 '??_C@_06GECNACHD@Name?3?5?$AA@':0x129e56c,  # derived from retail site; not in symbol_index
 '??_C@_0O@HLFAAHNO@?0?5Position?3?5?$CI?$AA@':0x129e574,  # derived from retail site; not in symbol_index
 '??_C@_0O@NDJJLBNB@?$CJ?0?5Sub?9mesh?3?5?$AA@':0x129e584,  # derived from retail site; not in symbol_index
 '??_C@_08DIPBKLJG@?0?5Bone?3?5?$AA@':0x129e594,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileHelperPointChunk@@6B@':0x129e5b4,  # derived from retail site; not in symbol_index
 '??_C@_0N@GEJHAAGE@Helper?5Point?$AA@':0x129e5e0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileHelperPointChunk_Methods_00aa96b0.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileHelperPointChunk@@UBE?BVCCharString@@XZ',address=0xaa96b0,size=425,instructions=113,relocations=39),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileHelperPointChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa98b0,size=92,instructions=36,relocations=7),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileHelperPointChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa9da0,size=100,instructions=36,relocations=5),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileHelperPointChunk@@UBE?BVCCharString@@XZ',address=0xaab130,size=25,instructions=9,relocations=2),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileHelperPointChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa9ba0,size=510,instructions=192,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-helper-point-chunk.json',[],
  'C3DMeshFileHelperPointChunk info/write/read/create/description')
 print('NATIVE_MESH_HELPER_POINT_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
