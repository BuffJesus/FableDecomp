"""Fresh VC71 parity gate: NResourceDirectoryNames user directories under My Games\\Fable: GetUserDataDir, GetSaveDirWithoutTrailingBlackslash, GetSaveDir, GetScoreboardImageCacheDir, GetPhotojournalDir, GetTattooUserDir."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetMyDocumentsDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ':0x41a430,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??H@YI?AVCWideString@@ABV0@PBG@Z':0x99bf30,
 '??_C@_13FPGAJAPJ@?$AA?2?$AA?$AA@':0x122de78,
 '??_C@_1CA@HHIGGKAD@?$AAM?$AAy?$AA?5?$AAG?$AAa?$AAm?$AAe?$AAs?$AA?2?$AAF?$AAa?$AAb?$AAl?$AAe?$AA?2?$AA?$AA@':0x122f504,
 '??_C@_1M@LKGGNHKO@?$AAS?$AAa?$AAv?$AAe?$AAs?$AA?$AA@':0x122f524,
 '??_C@_1BI@PJKJGFF@?$AAS?$AAc?$AAo?$AAr?$AAe?$AAb?$AAo?$AAa?$AAr?$AAd?$AA?2?$AA?$AA@':0x122f530,  # derived from retail site; not in symbol_index
 '??_C@_1BM@BFAMNCL@?$AAP?$AAh?$AAo?$AAt?$AAo?$AAj?$AAo?$AAu?$AAr?$AAn?$AAa?$AAl?$AA?2?$AA?$AA@':0x122f548,  # derived from retail site; not in symbol_index
 '??_C@_1BC@GBIOHDJA@?$AAT?$AAa?$AAt?$AAt?$AAo?$AAo?$AAs?$AA?2?$AA?$AA@':0x122f564,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/41/NResourceDirectoryNames_GetSaveDir_0041a540.cpp'
ROOTS=[
 dict(name='user_data',source=F,symbol='?GetUserDataDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a4c0,size=41,instructions=15,relocations=4),
 dict(name='save_no_slash',source=F,symbol='?GetSaveDirWithoutTrailingBlackslash@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a4f0,size=72,instructions=21,relocations=7),
 dict(name='save',source=F,symbol='?GetSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a540,size=99,instructions=27,relocations=10),
 dict(name='scoreboard',source=F,symbol='?GetScoreboardImageCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a5b0,size=72,instructions=21,relocations=7),
 dict(name='photojournal',source=F,symbol='?GetPhotojournalDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a600,size=72,instructions=21,relocations=7),
 dict(name='tattoo_user',source=F,symbol='?GetTattooUserDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a650,size=72,instructions=21,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-resource-user-dirs.json',[],
  'NResourceDirectoryNames user directories under My Games\\Fable: GetUserDataDir, GetSaveDirWithoutTrailingBlackslash, GetSaveDir, GetScoreboardImageCacheDir, GetPhotojournalDir, GetTattooUserDir')
 print('NATIVE_RESOURCE_USER_DIRS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
