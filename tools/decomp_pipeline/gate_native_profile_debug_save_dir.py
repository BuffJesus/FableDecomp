"""Fresh VC71 parity gate: CUserProfileManager::GetDebugSaveDir (L"Data\\\\" + GetSaveDir())."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ':0x41a540,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??H@YI?AVCWideString@@PBGABV0@@Z':0x99bff0,
 '??_C@_1M@ODADCJNO@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AA?$AA@':0x122dec8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/40/CUserProfileManager_GetDebugSaveDir_00406780.cpp'
ROOTS=[
 dict(name='debug_save',source=F,symbol='?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ',address=0x406780,size=44,instructions=15,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-profile-debug-save-dir.json',[],
  'CUserProfileManager::GetDebugSaveDir (L"Data\\\\" + GetSaveDir())')
 print('NATIVE_PROFILE_DEBUG_SAVE_DIR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
