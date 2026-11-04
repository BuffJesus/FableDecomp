"""Fresh VC71 parity gate: CWideString::operator!=(const CWideString&) over an inline == (identity, null, length, MemCmp_Unsigned16) and operator=(wchar_t) with UnassignString expanded (manifest label UnassignString at 0099B860 is wrong)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?MemCmp_Unsigned16@@YAHPBG0K@Z':0x403c60,
 '?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z':0x99b3c0,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CWideString_CharOperators_0099b0f0.cpp'
ROOTS=[
 dict(name='not_equal',source=F,symbol='??9CWideString@@QBE_NABV0@@Z',address=0x99b0f0,size=88,instructions=41,relocations=1),
 dict(name='assign_char',source=F,symbol='??4CWideString@@QAEABV0@G@Z',address=0x99b860,size=107,instructions=40,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-operators.json',[],
  'CWideString::operator!=(const CWideString&) over an inline == (identity, null, length, MemCmp_Unsigned16) and operator=(wchar_t) with UnassignString expanded (manifest label UnassignString at 0099B860 is wrong)')
 print('NATIVE_WIDE_STRING_OPERATORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
