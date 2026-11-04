"""Fresh VC71 parity gate: CAFile::SplitPath (narrow), GetRelativeFilePath and ValidatePathnameIsOnWriteableDrive (both expand SplitPath; the latter also ConvertPathToFullPath)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z':0x404720,
 '?Compare@?$CBasicString@D@@KIJPBD0@Z':0x411570,
 '?SplitPath@CAFile@@SIXABVCCharString@@PAV2@111@Z':0x9976d0,
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,
 '?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??BCCharString@@QBEPBDXZ':0x99e4c0,
 '??9CCharString@@QBE_NABV0@@Z':0x99e900,
 '??9CCharString@@QBE_NPBD@Z':0x99e960,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '??YCCharString@@QAEABV0@PBD@Z':0x99f100,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,
 '?SkipUntilString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9a20,
 '?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9ba0,
 '?SkipPastNextItemIfIs@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9e00,
 '?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z':0x9ba110,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '__splitpath':0xbfed52,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_01KICIPPFI@?2?$AA@':0x124c530,  # derived from retail site; not in symbol_index
 '??_C@_03LKILIBFM@?4?4?2?$AA@':0x129a7bc,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '?DebugUseReadOnlyDrives@CAFile@@2_NA':0x13bc9f8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_ValidatePathnameIsOnWriteableDrive_00999970.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='split_narrow',source=F,symbol='?SplitPath@CAFile@@SIXABVCCharString@@PAV2@111@Z',address=0x9976d0,size=168,instructions=46,relocations=8,flags=GS),
 dict(name='relative_path',source=F,symbol='?GetRelativeFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z',address=0x998570,size=1201,instructions=317,relocations=95,flags=GS),
 dict(name='validate_writeable',source=F,symbol='?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z',address=0x999970,size=192,instructions=50,relocations=14,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-validate-pathname.json',[],
  'CAFile::SplitPath (narrow), GetRelativeFilePath and ValidatePathnameIsOnWriteableDrive (both expand SplitPath; the latter also ConvertPathToFullPath)')
 print('NATIVE_FILE_VALIDATE_PATHNAME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
