"""Fresh VC71 parity gate for CDriveManager file operations (009E4B70 DeleteDirectory, 009E4BC0 DeleteFileW)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# Import slots from the PE import table; 0x99AF00 is the shared string-pointer
# conversion body (manifest label CCharString, byte-identical wide form).
ADDRESS={
 '?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z':0x999110,
 '??BCWideString@@QBEPBGXZ':0x99af00,
 '__imp__RemoveDirectoryW@4':0x143fd54,
 '__imp__DeleteFileW@4':0x143fe34,
 '__imp__GetLastError@0':0x143fd58,
 '??1CWideString@@QAE@XZ':0x99b510,
 '___security_cookie':0x139c8a8,
 '@__security_check_cookie@4':0xbfe9f9,
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,  # landed 00/99/CAFile_GetCurrentPath_00997f80.cpp
 '__wchdir':0xbfeafe,
 '__wsplitpath':0xbfed4c,
}
F=R/'rebuild/src/compiled/00/9e/CDriveManager_FileOperations_009e4b70.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']  # lib_drive_manager.cpp was built with /GS
ROOTS=[
 dict(name='delete_directory',source=F,symbol='?DeleteDirectory@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z',address=0x9e4b70,size=70,instructions=18,relocations=6,flags=GS),
 dict(name='delete_file',source=F,symbol='?DeleteFileW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z',address=0x9e4bc0,size=70,instructions=18,relocations=6,flags=GS),
 dict(name='drive_exists',source=F,symbol='?DriveExists@CDriveManager@@QAE?AW4EDriveReturn@@G@Z',address=0x9e4ae0,size=138,instructions=34,relocations=9,flags=GS),
 dict(name='get_drive_from_path',source=F,symbol='?GetDriveFromPath@CDriveManager@@AAEGPBG@Z',address=0x9e4c70,size=53,instructions=16,relocations=3,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-drive-manager.json',[],
  'Four genuine PDB-named roots (lib_drive_manager.cpp), byte-identical to retail; EDriveReturn enumerators from Ego_r.pdb. Retail mangles wchar_t natively (_W); the canonical tree builds without /Zc:wchar_t (G), which does not affect code bytes.')
 print('NATIVE_DRIVE_MANAGER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
