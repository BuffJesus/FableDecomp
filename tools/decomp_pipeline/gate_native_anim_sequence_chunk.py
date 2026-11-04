"""Fresh VC71 parity gate: C3DAnimFileSequenceChunk info/write/read/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteUBYTE@CDataOutputStream@@QAEXE@Z':0x993e50,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z':0x994070,
 '?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ':0x995340,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '?erase@?$vector@VCMatrix3x4@@V?$allocator@VCMatrix3x4@@@_STL@@@_STL@@QAEPAVCMatrix3x4@@PAV3@0@Z':0xaafd10,  # derived from retail site; not in symbol_index
 '?reserve@?$vector@VCMatrix3x4@@V?$allocator@VCMatrix3x4@@@_STL@@@_STL@@QAEXI@Z':0xaafd90,  # derived from retail site; not in symbol_index
 '?_M_insert_overflow@?$vector@VCMatrix3x4@@V?$allocator@VCMatrix3x4@@@_STL@@@_STL@@QAEXPAVCMatrix3x4@@ABV3@ABU__false_type@2@I_N@Z':0xaafe50,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '__snprintf':0xbfeb96,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
 '??_C@_04FAAHPKGJ@ASEQ?$AA@':0x129e7b4,  # derived from retail site; not in symbol_index
 '??_C@_0DL@CCBPEIFK@Bone?5Name?3?5?$CFs?4?5Bone?5Index?3?5?$CFli?0?5@':0x129e878,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileSequenceChunk@@6B@':0x129e8b8,  # derived from retail site; not in symbol_index
 '??_C@_0BD@OPOOKFFH@Animation?5Sequence?$AA@':0x129e8e4,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Methods_00aaf5b0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DAnimFileSequenceChunk@@UBE?BVCCharString@@XZ',address=0xaaf5b0,size=149,instructions=44,relocations=6,flags=GS),
 dict(name='write',source=F,symbol='?WriteToFile@C3DAnimFileSequenceChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaaf650,size=148,instructions=61,relocations=8,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DAnimFileSequenceChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaaf7a0,size=94,instructions=39,relocations=5,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DAnimFileSequenceChunk@@UBE?BVCCharString@@XZ',address=0xaaff90,size=25,instructions=9,relocations=2,flags=GS),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DAnimFileSequenceChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaaf850,size=657,instructions=245,relocations=12,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-sequence-chunk.json',[],
  'C3DAnimFileSequenceChunk info/write/read/create/description')
 print('NATIVE_ANIM_SEQUENCE_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
