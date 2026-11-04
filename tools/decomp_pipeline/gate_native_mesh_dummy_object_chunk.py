"""Fresh VC71 parity gate: C3DMeshFileDummyObjectChunk info/write/read/ctor/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z':0x994070,
 '?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ':0x995340,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?GetChunkInfo@C3DMeshFileHelperPointChunk@@UBE?BVCCharString@@XZ':0xaa96b0,
 '?WriteToFile@C3DMeshFileHelperPointChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa98b0,
 '?ReadFromFile@C3DMeshFileHelperPointChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa9ba0,
 '??0C3DMeshFileDummyObjectChunk@@QAE@XZ':0xaa9e10,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_02KEGNLNML@?0?5?$AA@':0x12397cc,  # derived from retail site; not in symbol_index
 '??_C@_04FGBDCMIO@HDMY?$AA@':0x129e524,  # derived from retail site; not in symbol_index
 '??_C@_0P@HMLDFAJ@?0?5Direction?3?5?$CI?$AA@':0x129e5a0,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileHelperPointChunk@@6B@':0x129e5b4,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileDummyObjectChunk@@6B@':0x129e5f4,  # derived from retail site; not in symbol_index
 '??_C@_0N@CLOPNBE@Dummy?5Object?$AA@':0x129e620,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileDummyObjectChunk_Methods_00aa9910.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileDummyObjectChunk@@UBE?BVCCharString@@XZ',address=0xaa9910,size=263,instructions=72,relocations=22),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileDummyObjectChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa9a70,size=95,instructions=36,relocations=7),
 dict(name='ctor',source=F,symbol='??0C3DMeshFileDummyObjectChunk@@QAE@XZ',address=0xaa9e10,size=74,instructions=24,relocations=5),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileDummyObjectChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaaa0c0,size=49,instructions=22,relocations=2),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileDummyObjectChunk@@UBE?BVCCharString@@XZ',address=0xaab180,size=25,instructions=9,relocations=2),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileDummyObjectChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa9e60,size=606,instructions=224,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-dummy-object-chunk.json',[],
  'C3DMeshFileDummyObjectChunk info/write/read/ctor/create/description')
 print('NATIVE_MESH_DUMMY_OBJECT_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
