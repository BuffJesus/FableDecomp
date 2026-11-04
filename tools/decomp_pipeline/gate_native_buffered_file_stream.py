"""Fresh VC71 parity gate: CBufferedFileDataOutputStream ctor, dtor (Flush expanded), SetPosition, GetPosition, Close, Write, Flush; supersedes the SetPosition/GetPosition shims."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Flush@CBufferedFileDataOutputStream@@QAEXXZ':0x994520,  # derived from retail site; not in symbol_index
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??_V@YAXPAX@Z':0xbfeb1c,
 '??_U@YAPAXI@Z':0xbfeb22,
 '??_C@_02PCIJFNDE@?$AN?6?$AA@':0x12397f8,
 '??_7CDataStream@@6B@':0x129a69c,
 '??_7CDataOutputStream@@6B@':0x129a6e0,
 '??_7CBufferedFileDataOutputStream@@6B@':0x129a710,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CBufferedFileDataOutputStream_00994520.cpp'
ROOTS=[
 dict(name='flush',source=F,symbol='?Flush@CBufferedFileDataOutputStream@@QAEXXZ',address=0x994520,size=75,instructions=32,relocations=0),
 dict(name='ctor',source=F,symbol='??0CBufferedFileDataOutputStream@@QAE@AAVCAFile@@J@Z',address=0x9944a0,size=90,instructions=30,relocations=6),
 dict(name='dtor',source=F,symbol='??1CBufferedFileDataOutputStream@@UAE@XZ',address=0x9947a0,size=105,instructions=37,relocations=5),
 dict(name='set_position',source=F,symbol='?SetPosition@CBufferedFileDataOutputStream@@UAEXK@Z',address=0x994500,size=10,instructions=3,relocations=0),
 dict(name='get_position',source=F,symbol='?GetPosition@CBufferedFileDataOutputStream@@UBEKXZ',address=0x994510,size=4,instructions=2,relocations=0),
 dict(name='close',source=F,symbol='?Close@CBufferedFileDataOutputStream@@UAEXXZ',address=0x994810,size=86,instructions=33,relocations=1),
 dict(name='write',source=F,symbol='?Write@CBufferedFileDataOutputStream@@UAEXPBXJ@Z',address=0x994870,size=189,instructions=81,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-buffered-file-stream.json',[],
  'CBufferedFileDataOutputStream ctor, dtor (Flush expanded), SetPosition, GetPosition, Close, Write, Flush; supersedes the SetPosition/GetPosition shims')
 print('NATIVE_BUFFERED_FILE_STREAM_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
