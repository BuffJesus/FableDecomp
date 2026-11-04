"""Fresh VC71 parity gate: CStreamingFontDataBank::ReadData (synchronous streaming-entry read) and StreamGlyph
(asynchronous high-priority read when the font manager loads glyphs asynchronously, else synchronous)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z':0x9d52b0,
 '?BeginReadData@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z':0x9d6060,
}
F=R/'rebuild/src/compiled/00/a5/CStreamingFontDataBank_StreamGlyph_00a5f010.cpp'
ROOTS=[
 dict(name='read_data',source=F,symbol='?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z',address=0xa5ef30,size=29,instructions=9,relocations=1),
 dict(name='stream_glyph',source=F,symbol='?StreamGlyph@CStreamingFontDataBank@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z',address=0xa5f010,size=118,instructions=40,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-streaming-font-bank-read.json',[],
  'CStreamingFontDataBank ReadData and StreamGlyph (streaming-entry glyph reads)')
 print('NATIVE_STREAMING_FONT_BANK_READ_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
