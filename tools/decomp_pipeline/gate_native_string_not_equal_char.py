"""Fresh VC71 parity gate: CCharString::operator!=(char) over the Ego_r inline operator==(char)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/99/CCharString_NotEqualChar_0099e520.cpp'
ROOTS=[
 dict(name='not_equal_char',source=F,symbol='??9CCharString@@QBE_ND@Z',address=0x99e520,size=127,instructions=55,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-not-equal-char.json',[],
  'CCharString::operator!=(char) over the Ego_r inline operator==(char)')
 print('NATIVE_STRING_NOT_EQUAL_CHAR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
