"""Fresh VC71 parity gate: NResourceDirectoryNames::GetDebugSaveDir (through CUserProfileManager::Get)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ':0x406780,  # derived from retail site; not in symbol_index
 '?Get@CUserProfileManager@@SIAAV1@XZ':0x40d2a0,
}
F=R/'rebuild/src/compiled/00/41/NResourceDirectoryNames_GetDebugSaveDir_0041a1a0.cpp'
ROOTS=[
 dict(name='debug_save_dir',source=F,symbol='?GetDebugSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a1a0,size=20,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-resource-debug-save-dir.json',[],
  'NResourceDirectoryNames::GetDebugSaveDir (through CUserProfileManager::Get)')
 print('NATIVE_RESOURCE_DEBUG_SAVE_DIR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
