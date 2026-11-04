"""Fresh VC71 parity gate: CDataOutputStream WriteString / WriteStringNewLine (text then NewLine) / WritePresizedString (length-prefixed)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CDataOutputStream_StringWriters_009959f0.cpp'
ROOTS=[
 dict(name='write_string',source=F,symbol='?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z',address=0x9959f0,size=46,instructions=22,relocations=1),
 dict(name='write_string_newline',source=F,symbol='?WriteStringNewLine@CDataOutputStream@@QAEXABVCCharString@@@Z',address=0x995a20,size=88,instructions=40,relocations=2),
 dict(name='write_presized',source=F,symbol='?WritePresizedString@CDataOutputStream@@QAEXABVCCharString@@@Z',address=0x995a80,size=86,instructions=42,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-stream-string-writers.json',[],
  'CDataOutputStream WriteString / WriteStringNewLine (text then NewLine) / WritePresizedString (length-prefixed)')
 print('NATIVE_DATA_STREAM_STRING_WRITERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
