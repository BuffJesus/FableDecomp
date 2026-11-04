"""Fresh VC71 parity gate: C3DAnimFileTimingEventChunk info/write/create/read/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_06GECNACHD@Name?3?5?$AA@':0x129e56c,  # derived from retail site; not in symbol_index
 '??_C@_04HIAFEPA@TMEV?$AA@':0x129e7ac,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileTimingEventChunk@@6B@':0x129e7f0,  # derived from retail site; not in symbol_index
 '??_C@_0N@JHCGFPNH@Timing?5Event?$AA@':0x129e81c,  # derived from retail site; not in symbol_index
 '??_C@_08KNAGJLKP@?0?5Time?3?5?$AA@':0x129e82c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DAnimFileTimingEventChunk_Methods_00aae190.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DAnimFileTimingEventChunk@@UBE?BVCCharString@@XZ',address=0xaae190,size=135,instructions=39,relocations=11),
 dict(name='write',source=F,symbol='?WriteToFile@C3DAnimFileTimingEventChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaae220,size=35,instructions=15,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DAnimFileTimingEventChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaae250,size=70,instructions=30,relocations=5),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DAnimFileTimingEventChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaae440,size=140,instructions=54,relocations=3),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DAnimFileTimingEventChunk@@UBE?BVCCharString@@XZ',address=0xaaeab0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-timing-event-chunk.json',[],
  'C3DAnimFileTimingEventChunk info/write/create/read/description')
 print('NATIVE_ANIM_TIMING_EVENT_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
