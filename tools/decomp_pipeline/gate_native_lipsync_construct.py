"""Fresh VC71 parity gate: CLipSyncDataBank constructor 0x00A42B80 (SetNonCached(false), owned CResourceBank) and SetPhonemeBank 0x00A427B0, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBankFileAsync@@QAE@XZ':0x9d5f80,
 '??0CResourceBank@@QAE@XZ':0x9fc5f0,
 '??2@YAPAXI@Z':0xbfea1a,
 '??_7CLipSyncDataBank@@6B@':0x129d3d4,
 '?Reset@?$CCountedPointer@VCResourceBank@@@@QAEXPAVCResourceBank@@@Z':0x992170,
 '?SetNonCached@CBankFileAsync@@QAEX_N@Z':0x9d5230,
}
ROOTS=[
 dict(name='clipsyncdatabank_construct',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??0CLipSyncDataBank@@QAE@XZ',address=0xa42b80,size=118,instructions=42,relocations=7),
 dict(name='clipsyncdatabank_setphonemebank',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?SetPhonemeBank@CLipSyncDataBank@@QAEXPAVCPhonemeDataBank@@@Z',address=0xa427b0,size=13,instructions=3,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-lipsync-construct.json',[],
  'CLipSyncDataBank constructor 0x00A42B80 (SetNonCached(false), owned CResourceBank) and SetPhonemeBank 0x00A427B0, exact ')
 print('NATIVE_LIPSYNC_CONSTRUCT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
