"""Fresh VC71 parity gate: CCharString::Truncate / Erase / SaveToFile (Ego_r in-place edits over CBasicString<char>::Truncate 0x009A0080)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?MakeOwnCopyOfData@CCharString@@AAEXXZ':0x99eaf0,
 '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,
 '?Truncate@?$CBasicString@D@@QAEXK@Z':0x9a0080,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/99/CCharString_Edit_0099f2a0.cpp'
ROOTS=[
 dict(name='truncate',source=F,symbol='?Truncate@CCharString@@QAEXK@Z',address=0x99f2a0,size=58,instructions=24,relocations=4),
 dict(name='erase',source=F,symbol='?Erase@CCharString@@QAEXJJ@Z',address=0x99f400,size=69,instructions=30,relocations=3),
 dict(name='save_to_file',source=F,symbol='?SaveToFile@CCharString@@QBEXAAVCAFile@@@Z',address=0x99e5f0,size=42,instructions=21,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-edits.json',[],
  'CCharString::Truncate / Erase / SaveToFile (Ego_r in-place edits over CBasicString<char>::Truncate 0x009A0080)')
 print('NATIVE_STRING_EDITS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
