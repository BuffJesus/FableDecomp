"""Fresh VC71 parity gate for CDateAndTime operators/conversions (009E9A30, 9B20, 9B80, 9BE0, 9D40)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={'__imp__SystemTimeToFileTime@8':0x143fd4c}  # KERNEL32 import slot (PE import table)
C=R/'rebuild/src/compiled/00/9e/CDateAndTime_Conversions_009e9a30.cpp'
E=R/'rebuild/src/compiled/00/9e/CDateAndTime_Equal_009e9ad0.cpp'
ROOTS=[
 dict(name='systemtime_ctor',source=C,symbol='??0CDateAndTime@@QAE@ABU_SYSTEMTIME@@@Z',address=0x9e9a30,size=56,instructions=17,relocations=0),
 dict(name='not_equal',source=E,symbol='??9CDateAndTime@@QBE_NABV0@@Z',address=0x9e9b20,size=89,instructions=34,relocations=0),
 dict(name='less',source=C,symbol='??MCDateAndTime@@QBE_NABV0@@Z',address=0x9e9b80,size=88,instructions=37,relocations=0),
 dict(name='greater',source=C,symbol='??OCDateAndTime@@QBE_NABV0@@Z',address=0x9e9be0,size=88,instructions=37,relocations=0),
 dict(name='to_file_time',source=C,symbol='?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ',address=0x9e9d40,size=109,instructions=28,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-date-and-time.json',['rebuild/tests/00/9e/CDateAndTime_Equal_009e9ad0_test.cpp still passes with operator!= added'],
  'Five genuine PDB-named roots, byte-identical to retail. operator!= shares the landed operator== TU (auto-inlined).')
 print('NATIVE_DATE_AND_TIME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
