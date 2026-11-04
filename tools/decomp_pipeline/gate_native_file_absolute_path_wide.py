"""Fresh VC71 parity gate: CAFile::GetAbsoluteFilePath wide form (narrow round trip through 0x998250)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z':0x998250,  # derived from retail site; not in symbol_index
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,
 '??1CCharString@@QAE@XZ':0x99eae0,
}
F=R/'rebuild/src/compiled/00/99/CAFile_GetAbsoluteFilePath_00998c30.cpp'
ROOTS=[
 dict(name='absolute_wide',source=F,symbol='?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z',address=0x998c30,size=92,instructions=31,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-absolute-path-wide.json',[],
  'CAFile::GetAbsoluteFilePath wide form (narrow round trip through 0x998250)')
 print('NATIVE_FILE_ABSOLUTE_PATH_WIDE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
