"""Fresh VC71 parity gate: CWaveFile / CWaveLoader (lib_wave_loader.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteUWORD@CDataOutputStream@@QAEXG@Z':0x993e80,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?GetLength@CMemoryDataOutputStream@@QBEKXZ':0x994b40,
 '?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x9959f0,
 '?PeekData@CMemoryDataOutputStream@@QBEPBXXZ':0x995e70,
 '??0CMemoryDataOutputStream@@QAE@XZ':0x996af0,
 '??1CMemoryDataOutputStream@@UAE@XZ':0x996b30,
 '?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z':0x996cc0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4?$vector@FV?$allocator@F@_STL@@@_STL@@QAEAAV01@ABV01@@Z':0xaade80,
 '?DeleteData@?$CCountedPointer@VCWaveFile@@@@CIXPAX@Z':0xaadfc0,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??2@YAPAXI@Z':0xbfea1a,
 '??_C@_04EPCGJKAP@RIFF?$AA@':0x129e780,
 '??_C@_04PJOLNDGD@data?$AA@':0x129e788,
 '??_C@_04CAJPEBPP@fmt?5?$AA@':0x129e790,
 '??_C@_04HFHJMNCD@WAVE?$AA@':0x129e798,
}
F0=R/'rebuild/src/compiled/00/aa/CWaveFile_Methods_00aad120.cpp'
F1=R/'rebuild/src/compiled/00/aa/CCountedPointerCWaveFile_Reset_00aadff0.cpp'
ROOTS=[
 dict(name='get_byte_length',source=F0,symbol='?GetByteLength@CWaveFile@@QBEKXZ',address=0xaad120,size=11,instructions=5,relocations=0),
 dict(name='peek_audio_data',source=F0,symbol='?PeekAudioData@CWaveFile@@QBEPBFXZ',address=0xaad130,size=19,instructions=9,relocations=0),
 dict(name='save_wave_file',source=F0,symbol='?SaveWaveFile@CWaveLoader@@QAEXABVCWaveFile@@AAVCDataOutputStream@@@Z',address=0xaad150,size=450,instructions=135,relocations=37),
 dict(name='wave_file_ctor',source=F0,symbol='??0CWaveFile@@QAE@XZ',address=0xaad320,size=19,instructions=8,relocations=0),
 dict(name='initialise',source=F0,symbol='?Initialise@CWaveFile@@QAEXKKABV?$CArray@F@@@Z',address=0xaad590,size=53,instructions=22,relocations=1),
 dict(name='wave_file_pointer_reset',source=F1,symbol='?Reset@?$CCountedPointer@VCWaveFile@@@@QAEXPAVCWaveFile@@@Z',address=0xaadff0,size=103,instructions=36,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wave-loader.json',[],
  'CWaveFile / CWaveLoader (lib_wave_loader.cpp)')
 print('NATIVE_WAVE_LOADER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
