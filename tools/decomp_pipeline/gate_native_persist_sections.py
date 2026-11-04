"""Fresh VC71 parity gate: CPersistContext StartSection / EndSection (binary: name + length slot patched at the end; text: START_/END_ lines)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '??_C@_02PCIJFNDE@?$AN?6?$AA@':0x12397f8,
 '??_C@_04FMMHFHOO@?$AN?6?$AN?6?$AA@':0x129aa38,  # derived from retail site; not in symbol_index
 '??_C@_06LFLAOIGN@START_?$AA@':0x129b31c,  # derived from retail site; not in symbol_index
 '??_C@_04JILFJEFC@END_?$AA@':0x129b324,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9b/CPersistContext_Sections_009bab60.cpp'
ROOTS=[
 dict(name='start_section',source=F,symbol='?StartSection@CPersistContext@@QAEXAAVCPersistContextWriteSection@@ABVCCharString@@@Z',address=0x9bab60,size=134,instructions=47,relocations=10),
 dict(name='end_section',source=F,symbol='?EndSection@CPersistContext@@QAEXAAVCPersistContextWriteSection@@@Z',address=0x9babf0,size=144,instructions=54,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-persist-sections.json',[],
  'CPersistContext StartSection / EndSection (binary: name + length slot patched at the end; text: START_/END_ lines)')
 print('NATIVE_PERSIST_SECTIONS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
