"""Fresh VC71 parity gate: CBankFile::SplitSymbol (retail copies the whole symbol out; slip kept), IsSymbolPublic, ConvertPathToFilename."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z':0x997620,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??YCWideString@@QAEABV0@ABV0@@Z':0x99b8d0,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '?FindCharPos@CCharString@@QBEJD@Z':0x99e5a0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?GetSubString@CCharString@@QBE?AV1@KK@Z':0x99ec70,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/9c/CBankFile_SymbolHelpers_009cbaa0.cpp'
ROOTS=[
 dict(name='split_symbol',source=F,symbol='?SplitSymbol@CBankFile@@QAEXABVCCharString@@PAV2@1@Z',address=0x9cbaa0,size=212,instructions=71,relocations=17),
 dict(name='symbol_public',source=F,symbol='?IsSymbolPublic@CBankFile@@QBE_NABVCCharString@@@Z',address=0x9cbdf0,size=20,instructions=8,relocations=1),
 dict(name='path_to_filename',source=F,symbol='?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z',address=0x9cbe10,size=121,instructions=36,relocations=9),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-symbol-helpers.json',[],
  'CBankFile::SplitSymbol (retail copies the whole symbol out; slip kept), IsSymbolPublic, ConvertPathToFilename')
 print('NATIVE_BANK_SYMBOL_HELPERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
