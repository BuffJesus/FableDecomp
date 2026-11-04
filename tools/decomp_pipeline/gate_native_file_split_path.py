"""Fresh VC71 parity gate: lib_file_abstract path ops sharing inline bodies: NFile::ConvertDriveLetterToNumber, CAFile::SplitPath (wide), MakePath, SetFilenameExtension, GetFilenameFromPathname, CreateDirectoryW, CreateDirectoryForFile."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z':0x997620,
 '?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z':0x997780,
 '?GetItemFromPath@NFile@@YI_NPBGKPAG@Z':0x997880,  # derived from retail site; not in symbol_index
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,
 '?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,
 '?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,
 '?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z':0x9994a0,  # derived from retail site; not in symbol_index
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??BCWideString@@QBEPBGXZ':0x99af00,
 '?GetAsCharArray@CWideString@@QBEPBGXZ':0x99af10,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CWideString@@QAE@PBG@Z':0x99b6b0,
 '??4CWideString@@QAEABV0@PBG@Z':0x99b800,
 '??H@YI?AVCWideString@@ABV0@0@Z':0x99be70,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '__wchdir':0xbfeafe,  # derived from retail site; not in symbol_index
 '__wsplitpath':0xbfed4c,  # derived from retail site; not in symbol_index
 '__wmakepath':0xbfed58,  # derived from retail site; not in symbol_index
 '__wmkdir':0xbfed64,  # derived from retail site; not in symbol_index
 '__chdrive':0xbfed6a,  # derived from retail site; not in symbol_index
 '__getdrive':0xbfed70,  # derived from retail site; not in symbol_index
 '??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_SplitPath_00997620.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='drive_letter',source=F,symbol='?ConvertDriveLetterToNumber@NFile@@YIJG@Z',address=0x997470,size=41,instructions=16,relocations=0,flags=GS),
 dict(name='split_wide',source=F,symbol='?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z',address=0x997620,size=168,instructions=46,relocations=8,flags=GS),
 dict(name='make_path',source=F,symbol='?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z',address=0x997780,size=121,instructions=34,relocations=8,flags=GS),
 dict(name='set_extension',source=F,symbol='?SetFilenameExtension@CAFile@@SI?AVCWideString@@ABV2@0@Z',address=0x997800,size=122,instructions=39,relocations=8,flags=GS),
 dict(name='filename_from_path',source=F,symbol='?GetFilenameFromPathname@CAFile@@SI?AVCWideString@@ABV2@@Z',address=0x997930,size=172,instructions=48,relocations=11,flags=GS),
 dict(name='create_directory',source=F,symbol='?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z',address=0x9994a0,size=373,instructions=115,relocations=22,flags=GS),
 dict(name='create_directory_for_file',source=F,symbol='?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z',address=0x999620,size=308,instructions=80,relocations=25,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-split-path.json',[],
  'lib_file_abstract path ops sharing inline bodies: NFile::ConvertDriveLetterToNumber, CAFile::SplitPath (wide), MakePath, SetFilenameExtension, GetFilenameFromPathname, CreateDirectoryW, CreateDirectoryForFile')
 print('NATIVE_FILE_SPLIT_PATH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
