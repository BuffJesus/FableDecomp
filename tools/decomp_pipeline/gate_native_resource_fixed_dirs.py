"""Fresh VC71 parity gate: NResourceDirectoryNames getters under their Ego_r names (Misc, FontBank, Cache, BoneConfig, Sound, HDDialogue, DVD/HD Sound/Music, StaticMap); supersede FableGet*/S::M stand-ins."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetLanguageFolder@NLocalisation@@YI?AVCCharString@@XZ':0x415530,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CWideString@@QAE@PBG@Z':0x99b6b0,
 '??H@YI?AVCWideString@@ABV0@PBG@Z':0x99bf30,
 '??H@YI?AVCWideString@@PBGABV0@@Z':0x99bff0,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??_C@_1BG@JBHLEDNN@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAl?$AAa?$AAn?$AAg?$AA?2?$AA?$AA@':0x122dba8,
 '??_C@_13FPGAJAPJ@?$AA?2?$AA?$AA@':0x122de78,
 '??_C@_1BK@GEPDHCGD@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAL?$AAe?$AAv?$AAe?$AAl?$AAs?$AA?2?$AA?$AA@':0x122f3b4,
 '??_C@_1BG@HNBHPCFC@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAM?$AAi?$AAs?$AAc?$AA?2?$AA?$AA@':0x122f3e8,
 '??_C@_1BI@MOMDIJEO@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAC?$AAa?$AAc?$AAh?$AAe?$AA?2?$AA?$AA@':0x122f43c,
 '??_C@_1BI@BIJMEMB@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAB?$AAo?$AAn?$AAe?$AAs?$AA?2?$AA?$AA@':0x122f4d4,  # derived from retail site; not in symbol_index
 '??_C@_1BI@GGMDENLB@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAS?$AAo?$AAu?$AAn?$AAd?$AA?2?$AA?$AA@':0x122f4ec,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/41/NResourceDirectoryNames_FixedDirs_0041a0a0.cpp'
ROOTS=[
 dict(name='misc',source=F,symbol='?GetMiscDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a0a0,size=17,instructions=7,relocations=2),
 dict(name='font_bank',source=F,symbol='?GetFontBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a120,size=90,instructions=27,relocations=9),
 dict(name='cache',source=F,symbol='?GetCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a180,size=17,instructions=7,relocations=2),
 dict(name='bone_config',source=F,symbol='?GetBoneConfigDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a290,size=17,instructions=7,relocations=2),
 dict(name='sound',source=F,symbol='?GetSoundDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a2b0,size=17,instructions=7,relocations=2),
 dict(name='hd_dialogue',source=F,symbol='?GetHDDialogueDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a330,size=90,instructions=27,relocations=9),
 dict(name='dvd_sound',source=F,symbol='?GetDVDSoundDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a390,size=17,instructions=7,relocations=2),
 dict(name='hd_sound',source=F,symbol='?GetHDSoundDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a3b0,size=17,instructions=7,relocations=2),
 dict(name='hd_music',source=F,symbol='?GetHDMusicDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a3d0,size=17,instructions=7,relocations=2),
 dict(name='dvd_music',source=F,symbol='?GetDVDMusicDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a3f0,size=17,instructions=7,relocations=2),
 dict(name='static_map',source=F,symbol='?GetStaticMapDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a410,size=17,instructions=7,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-resource-fixed-dirs.json',[],
  'NResourceDirectoryNames getters under their Ego_r names (Misc, FontBank, Cache, BoneConfig, Sound, HDDialogue, DVD/HD Sound/Music, StaticMap); supersede FableGet*/S::M stand-ins')
 print('NATIVE_RESOURCE_FIXED_DIRS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
