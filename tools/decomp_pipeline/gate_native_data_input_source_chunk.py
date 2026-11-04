"""Fresh VC71 parity gate: CDataInputStream SetPositionAssumeWithinSourceChunk, CanSeekToPosWithinSourceChunk, OpenStream."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/99/CDataInputStream_SourceChunk_00993c10.cpp'
ROOTS=[
 dict(name='set_position_in_chunk',source=F,symbol='?SetPositionAssumeWithinSourceChunk@CDataInputStream@@IAEXK@Z',address=0x993c10,size=39,instructions=17,relocations=0),
 dict(name='can_seek_in_chunk',source=F,symbol='?CanSeekToPosWithinSourceChunk@CDataInputStream@@IAE_NJ@Z',address=0x993c40,size=32,instructions=11,relocations=0),
 dict(name='open_stream',source=F,symbol='?OpenStream@CDataInputStream@@QAEXPBEJJ@Z',address=0x993c60,size=32,instructions=10,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-input-source-chunk.json',[],
  'CDataInputStream SetPositionAssumeWithinSourceChunk, CanSeekToPosWithinSourceChunk, OpenStream')
 print('NATIVE_DATA_INPUT_SOURCE_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
