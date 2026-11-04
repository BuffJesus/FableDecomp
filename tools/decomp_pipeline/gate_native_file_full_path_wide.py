"""Fresh VC71 parity gate: CAFile::ConvertPathToFullPath wide form: ExpandPath(path, GetCurrentPath()) (replaces a stand-in with a fake scratch argument)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,
 '?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,
 '??1CWideString@@QAE@XZ':0x99b510,
}
F=R/'rebuild/src/compiled/00/99/global_ConvertPathToFullPath_00999110.cpp'
ROOTS=[
 dict(name='full_path_wide',source=F,symbol='?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z',address=0x999110,size=41,instructions=18,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-full-path-wide.json',[],
  'CAFile::ConvertPathToFullPath wide form: ExpandPath(path, GetCurrentPath()) (replaces a stand-in with a fake scratch argument)')
 print('NATIVE_FILE_FULL_PATH_WIDE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
