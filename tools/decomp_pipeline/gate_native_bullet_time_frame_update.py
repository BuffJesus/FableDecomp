"""Fresh VC71 parity gate: CBulletTimeManager::FrameUpdate (0x004C5E90, empty), exact"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='frame_update',source=R/'rebuild/src/compiled/00/4c/CBulletTimeManager_FrameUpdate_004c5e90.cpp',symbol='?FrameUpdate@CBulletTimeManager@@QAEXXZ',address=0x4c5e90,size=1,instructions=1,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bullet-time-frame-update.json',[],
  'CBulletTimeManager::FrameUpdate (0x004C5E90, empty), exact')
 print('NATIVE_BULLET_TIME_FRAME_UPDATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
