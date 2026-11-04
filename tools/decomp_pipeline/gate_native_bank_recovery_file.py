"""Fresh VC71 parity gate: CBankFile::GetRecoveryFilePath (<bank>.rec) and DeleteRecoveryFile."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z':0x997620,
 '?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??BCWideString@@QBEPBGXZ':0x99af10,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CWideString@@QAE@ABV0@@Z':0x99b720,
 '??H@YI?AVCWideString@@ABV0@0@Z':0x99be70,
 '??H@YI?AVCWideString@@ABV0@PBG@Z':0x99bf30,
 '?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ':0x9cbfe0,  # derived from retail site; not in symbol_index
 '??_C@_19NCDOPEAD@?$AA?4?$AAr?$AAe?$AAc?$AA?$AA@':0x129b52c,  # derived from retail site; not in symbol_index
 '__imp__DeleteFileW@4':0x143fe34,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9c/CBankFile_RecoveryFile_009cbfe0.cpp'
ROOTS=[
 dict(name='recovery_path',source=F,symbol='?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ',address=0x9cbfe0,size=207,instructions=59,relocations=16),
 dict(name='delete_recovery',source=F,symbol='?DeleteRecoveryFile@CBankFile@@AAEXXZ',address=0x9cc160,size=47,instructions=16,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-recovery-file.json',[],
  'CBankFile::GetRecoveryFilePath (<bank>.rec) and DeleteRecoveryFile')
 print('NATIVE_BANK_RECOVERY_FILE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
