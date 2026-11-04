"""Fresh VC71 parity gate: CCharString::MakeOwnCopyOfDataOrEmptyString (FableWin protected member): MakeOwnCopyOfData expanded, else an empty block via the FableWin CCharStringData(long,char) / CBasicString(unsigned long,char) ctors."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?UnassignString@CCharString@@AAEXXZ':0x99e9b0,
 '?SetString@?$CBasicString@D@@IAEXKD@Z':0x9a0390,  # derived from retail site; not in symbol_index
 '??0?$CBasicString@D@@QAE@PBD@Z':0x9a0590,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CCharString_MakeOwnCopyOfDataOrEmptyString_0099eb40.cpp'
ROOTS=[
 dict(name='own_copy_or_empty',source=F,symbol='?MakeOwnCopyOfDataOrEmptyString@CCharString@@IAEXXZ',address=0x99eb40,size=169,instructions=65,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-own-copy-or-empty.json',[],
  'CCharString::MakeOwnCopyOfDataOrEmptyString (FableWin protected member): MakeOwnCopyOfData expanded, else an empty block via the FableWin CCharStringData(long,char) / CBasicString(unsigned long,char) ctors')
 print('NATIVE_STRING_OWN_COPY_OR_EMPTY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
