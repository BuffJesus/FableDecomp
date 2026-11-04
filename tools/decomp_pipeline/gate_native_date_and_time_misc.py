"""Fresh VC71 parity gate: CDateAndTime >=, <=, Initialise(SYSTEMTIME), Initialise(CDateAndTime), ToString (lib_date_and_time.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??MCDateAndTime@@QBE_NABV0@@Z':0x9e9b80,
 '??OCDateAndTime@@QBE_NABV0@@Z':0x9e9be0,
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_0CA@FCLDKIHO@?$FL?$CF02d?1?$CF02d?1?$CF02d?5?$CF02d?3?$CF02d?3?$CF02d?$FN?$AA@':0x129c3a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CDateAndTime_Misc_009e9c40.cpp'
ROOTS=[
 dict(name='ge',source=F,symbol='??PCDateAndTime@@QBE_NABV0@@Z',address=0x9e9c40,size=18,instructions=7,relocations=1),
 dict(name='init_system_time',source=F,symbol='?Initialise@CDateAndTime@@QAEXABU_SYSTEMTIME@@@Z',address=0x9e9c70,size=54,instructions=16,relocations=0),
 dict(name='init_copy',source=F,symbol='?Initialise@CDateAndTime@@QAEXABV1@@Z',address=0x9e9d10,size=47,instructions=16,relocations=0),
 dict(name='le',source=F,symbol='??NCDateAndTime@@QBE_NABV0@@Z',address=0x9e9e10,size=18,instructions=7,relocations=1),
 dict(name='to_string',source=F,symbol='?ToString@CDateAndTime@@QBE?AVCCharString@@XZ',address=0x9e9e30,size=125,instructions=50,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-date-and-time-misc.json',[],
  'CDateAndTime >=, <=, Initialise(SYSTEMTIME), Initialise(CDateAndTime), ToString (lib_date_and_time.cpp)')
 print('NATIVE_DATE_AND_TIME_MISC_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
