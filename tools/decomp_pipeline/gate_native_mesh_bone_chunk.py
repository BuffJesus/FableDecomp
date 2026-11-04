"""Fresh VC71 parity gate: C3DMeshFileBoneChunk write/create/read/info/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z':0x994070,
 '?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ':0x995340,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '__snprintf':0xbfeb96,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
 '??_C@_04CPJDFACK@BONE?$AA@':0x129dedc,  # derived from retail site; not in symbol_index
 '??_C@_0DM@PFEJPEPO@Name?3?5?$CFs?0?5Index?3?5?$CFli?0?5Parent?3?5?$CFl@':0x129e940,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileBoneChunk@@6B@':0x129eb9c,  # derived from retail site; not in symbol_index
 '??_C@_04CCGIJNFG@Bone?$AA@':0x129ebc8,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileBoneChunk_Methods_00ab9ea0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileBoneChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xab9ea0,size=92,instructions=36,relocations=7,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileBoneChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xab9f00,size=87,instructions=37,relocations=5,flags=GS),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileBoneChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaba170,size=474,instructions=178,relocations=8,flags=GS),
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileBoneChunk@@UBE?BVCCharString@@XZ',address=0xabadd0,size=128,instructions=38,relocations=6,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileBoneChunk@@UBE?BVCCharString@@XZ',address=0xabd400,size=25,instructions=9,relocations=2,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-bone-chunk.json',[],
  'C3DMeshFileBoneChunk write/create/read/info/description')
 print('NATIVE_MESH_BONE_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
