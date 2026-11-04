"""Fresh VC71 parity gate: CDataOutputStream value writers (Ego_r lib_data_stream order) incl. WriteColour, WritePixelFormat, Write2DVector, WriteMatrix3x4; supersedes stand-in landings."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteRepeatedByte@CDataOutputStream@@QAEXEJ@Z':0x994230,
 '??BCWideString@@QBEPBGXZ':0x99af10,
 '?GetLength@CWideString@@QBEJXZ':0x99b220,
 '?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ':0x9e3870,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CDataOutputStream_Writers_00993e50.cpp'
ROOTS=[
 dict(name='ubyte',source=F,symbol='?WriteUBYTE@CDataOutputStream@@QAEXE@Z',address=0x993e50,size=15,instructions=6,relocations=0),
 dict(name='sbyte',source=F,symbol='?WriteSBYTE@CDataOutputStream@@QAEXC@Z',address=0x993e60,size=15,instructions=6,relocations=0),
 dict(name='cbyte',source=F,symbol='?WriteCBYTE@CDataOutputStream@@QAEXD@Z',address=0x993e70,size=15,instructions=6,relocations=0),
 dict(name='uword',source=F,symbol='?WriteUWORD@CDataOutputStream@@QAEXG@Z',address=0x993e80,size=15,instructions=6,relocations=0),
 dict(name='sword',source=F,symbol='?WriteSWORD@CDataOutputStream@@QAEXF@Z',address=0x993e90,size=15,instructions=6,relocations=0),
 dict(name='uhuge',source=F,symbol='?WriteUHUGE@CDataOutputStream@@QAEX_K@Z',address=0x993ec0,size=15,instructions=6,relocations=0),
 dict(name='shuge',source=F,symbol='?WriteSHUGE@CDataOutputStream@@QAEX_J@Z',address=0x993ed0,size=15,instructions=6,relocations=0),
 dict(name='float',source=F,symbol='?WriteFloat@CDataOutputStream@@QAEXM@Z',address=0x993ee0,size=15,instructions=6,relocations=0),
 dict(name='double',source=F,symbol='?WriteDouble@CDataOutputStream@@QAEXN@Z',address=0x993ef0,size=15,instructions=6,relocations=0),
 dict(name='wstring',source=F,symbol='?WriteNullTerminatedWString@CDataOutputStream@@QAEXABVCWideString@@@Z',address=0x993f00,size=46,instructions=22,relocations=2),
 dict(name='vector3',source=F,symbol='?Write3DVector@CDataOutputStream@@QAEXABVC3DVector@@@Z',address=0x993fd0,size=73,instructions=30,relocations=0),
 dict(name='angles3',source=F,symbol='?Write3DAngles@CDataOutputStream@@QAEXABVC3DAngles@@@Z',address=0x994020,size=73,instructions=30,relocations=0),
 dict(name='align',source=F,symbol='?AlignData@CDataOutputStream@@QAEXJ@Z',address=0x9942d0,size=43,instructions=18,relocations=1),
 dict(name='colour',source=F,symbol='?WriteColour@CDataOutputStream@@QAEXABVCRGBColour@@@Z',address=0x993f30,size=46,instructions=15,relocations=0),
 dict(name='vector_2d',source=F,symbol='?Write2DVector@CDataOutputStream@@QAEXABVC2DVector@@@Z',address=0x993f90,size=52,instructions=22,relocations=0),
 dict(name='matrix',source=F,symbol='?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z',address=0x994070,size=262,instructions=102,relocations=0),
 dict(name='pixel_format',source=F,symbol='?WritePixelFormat@CDataOutputStream@@QAEXABVCPixelFormat@@@Z',address=0x993f60,size=41,instructions=16,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-stream-writers.json',[],
  'CDataOutputStream value writers (Ego_r lib_data_stream order) incl. WriteColour, WritePixelFormat, Write2DVector, WriteMatrix3x4; supersedes stand-in landings')
 print('NATIVE_DATA_STREAM_WRITERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
