"""Fresh VC71 parity gate: GFUIntToCharString / GFUHUGEToCharString (Ego_r parameter n; CCharString lifetime members expanded as in GFIntToCharString)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?UnassignString@CCharString@@AAEXXZ':0x99e9b0,
 '?Sprintf@CCharString@@QAAXPBDZZ':0x99f1f0,
 '??_C@_02GMHACPFF@?$CFu?$AA@':0x129ab00,  # derived from retail site; not in symbol_index
 '??_C@_05OINFJHGD@?$CFI64u?$AA@':0x129ab04,  # derived from retail site; not in symbol_index
 '?g_CCharStringInstanceCount_013BD800@@3HA':0x13bd800,
}
F=R/'rebuild/src/compiled/00/99/Global_GFUIntToCharString_0099f8b0.cpp'
ROOTS=[
 dict(name='uint',source=F,symbol='?GFUIntToCharString@@YI?AVCCharString@@K@Z',address=0x99f8b0,size=116,instructions=36,relocations=9),
 dict(name='uhuge',source=F,symbol='?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z',address=0x99f930,size=127,instructions=39,relocations=9),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-unsigned-format.json',[],
  'GFUIntToCharString / GFUHUGEToCharString (Ego_r parameter n; CCharString lifetime members expanded as in GFIntToCharString)')
 print('NATIVE_STRING_UNSIGNED_FORMAT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
