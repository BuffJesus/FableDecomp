"""Fresh VC71 parity gate: C3DAnimFileXSequenceChunk write/create/read/description (raw compressed bytes)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXIABE@Z':0x486189,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04DNPHAPJK@XSEQ?$AA@':0x129d4f8,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileXSequenceChunk@@6B@':0x129e194,  # derived from retail site; not in symbol_index
 '??_C@_0BO@MIMLPGNP@Compressed?5Animation?5Sequence?$AA@':0x129e1c0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DAnimFileXSequenceChunk_Methods_00aa4650.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DAnimFileXSequenceChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4650,size=43,instructions=22,relocations=0),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DAnimFileXSequenceChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa46b0,size=83,instructions=31,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DAnimFileXSequenceChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa48d0,size=150,instructions=66,relocations=2),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DAnimFileXSequenceChunk@@UBE?BVCCharString@@XZ',address=0xaa49d0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-xsequence-chunk.json',[],
  'C3DAnimFileXSequenceChunk write/create/read/description (raw compressed bytes)')
 print('NATIVE_ANIM_XSEQUENCE_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
