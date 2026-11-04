"""Fresh VC71 parity gate: CAFile queries through ExpandPath(path, GetCurrentPath()): ConvertPathToFullPath (narrow), GetFileModifiedTimeAsSomeWackyWin32BigNumber (ftLastWriteTime via _allmul), IsFileReadOnly, SetFileReadOnly, IsPathFile."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,
 '?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,
 '?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??BCWideString@@QBEPBGXZ':0x99af00,
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,
 '__allmul':0xbfeb50,  # derived from retail site; not in symbol_index
 '__imp__SetFileAttributesW@8':0x143fdb4,  # derived from retail site; not in symbol_index
 '__imp__GetFileAttributesExW@12':0x143fdb8,  # derived from retail site; not in symbol_index
 '__imp__GetFileAttributesW@4':0x143fdbc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_FileQueries_009990b0.cpp'
ROOTS=[
 dict(name='full_path_narrow',source=F,symbol='?ConvertPathToFullPath@CAFile@@SI?AVCCharString@@ABV2@@Z',address=0x9990b0,size=85,instructions=26,relocations=7),
 dict(name='modified_big',source=F,symbol='?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile@@SI_KABVCWideString@@@Z',address=0x999320,size=137,instructions=46,relocations=10),
 dict(name='read_only',source=F,symbol='?IsFileReadOnly@CAFile@@SI_NABVCWideString@@@Z',address=0x9993b0,size=103,instructions=32,relocations=9),
 dict(name='set_read_only',source=F,symbol='?SetFileReadOnly@CAFile@@SIXABVCWideString@@_N@Z',address=0x999420,size=126,instructions=40,relocations=11),
 dict(name='is_path_file',source=F,symbol='?IsPathFile@CAFile@@SI_NABVCWideString@@@Z',address=0x999a30,size=123,instructions=39,relocations=11),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-queries.json',[],
  'CAFile queries through ExpandPath(path, GetCurrentPath()): ConvertPathToFullPath (narrow), GetFileModifiedTimeAsSomeWackyWin32BigNumber (ftLastWriteTime via _allmul), IsFileReadOnly, SetFileReadOnly, IsPathFile')
 print('NATIVE_FILE_QUERIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
