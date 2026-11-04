"""Fresh VC71 parity gate: CCharString::operator!=(const CCharString&) (negated inline ==)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Compare@?$CBasicString@D@@KIJPBD0@Z':0x411570,
}
F=R/'rebuild/src/compiled/00/99/CCharString_NotEqual_0099e900.cpp'
ROOTS=[
 dict(name='not_equal',source=F,symbol='??9CCharString@@QBE_NABV0@@Z',address=0x99e900,size=83,instructions=35,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-not-equal-string.json',[],
  'CCharString::operator!=(const CCharString&) (negated inline ==)')
 print('NATIVE_STRING_NOT_EQUAL_STRING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
