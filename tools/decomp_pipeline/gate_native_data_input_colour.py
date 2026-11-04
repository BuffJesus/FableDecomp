"""Fresh VC71 parity gate: CDataInputStream::ReadColour (4 bytes via the inline Read, rebuilt R,G,B,A) and ReadPixelFormat (memset CPixelFormatInit, Initialise via the inline ctor)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?Initialise@CPixelFormat@@QAEXABVCPixelFormatInit@@@Z':0x9e3640,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CDataInputStream_ReadColour_009945e0.cpp'
ROOTS=[
 dict(name='read_colour',source=F,symbol='?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ',address=0x9945e0,size=138,instructions=52,relocations=1),
 dict(name='read_pixel_format',source=F,symbol='?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ',address=0x994670,size=137,instructions=50,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-input-colour.json',[],
  'CDataInputStream::ReadColour (4 bytes via the inline Read, rebuilt R,G,B,A) and ReadPixelFormat (memset CPixelFormatInit, Initialise via the inline ctor)')
 print('NATIVE_DATA_INPUT_COLOUR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
