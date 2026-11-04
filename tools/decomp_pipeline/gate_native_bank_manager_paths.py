"""Fresh VC71 parity gate: CBankFileManager LookupBankAlias / GetDevBankFilePath (throws on unknown bank) / GetDevHeaderFilePath / GetRetailHeaderFilePath."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetLength@CWideString@@QBEJXZ':0x99b220,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CWideString@@QAE@PBG@Z':0x99b6b0,
 '??0CWideString@@QAE@ABV0@@Z':0x99b720,
 '??H@YI?AVCWideString@@ABV0@0@Z':0x99be70,
 '??BCCharString@@QBEPBDXZ':0x99e4c0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?Find@CBankInfoMap@CBankFileManager@@QBEPAUCNode@12@ABVCCharString@@@Z':0x9ab560,
 '?Find@CBasicRetailBankAliasMap@@QBEPAUCNode@1@ABVCCharString@@@Z':0x9ab5d0,
 '__CxxThrowException@8':0xbfeb84,  # derived from retail site; not in symbol_index
 '??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,
 '??_C@_0BB@KFODCBHH@?5bank?5not?5found?$CB?$AA@':0x129b01c,  # derived from retail site; not in symbol_index
 '__TI2?AVCGenericException@@':0x13692f8,
}
F=R/'rebuild/src/compiled/00/9a/CBankFileManager_HeaderPaths_009a7e20.cpp'
ROOTS=[
 dict(name='dev_bank',source=F,symbol='?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z',address=0x9a7ca0,size=145,instructions=52,relocations=10),
 dict(name='dev_header',source=F,symbol='?GetDevHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z',address=0x9a7e20,size=171,instructions=62,relocations=12),
 dict(name='retail_header',source=F,symbol='?GetRetailHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z',address=0x9a7ed0,size=171,instructions=62,relocations=12),
 dict(name='lookup_alias',source=F,symbol='?LookupBankAlias@CBankFileManager@@QBE?AVCCharString@@ABV2@@Z',address=0x9a7c60,size=58,instructions=25,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-manager-paths.json',[],
  'CBankFileManager LookupBankAlias / GetDevBankFilePath (throws on unknown bank) / GetDevHeaderFilePath / GetRetailHeaderFilePath')
 print('NATIVE_BANK_MANAGER_PATHS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
