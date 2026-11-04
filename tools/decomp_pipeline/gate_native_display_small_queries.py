"""Fresh VC71 parity gate: CDisplayManager small device/state queries and PC no-op tile/interval hooks."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?BlitRenderTargetSurfaceToTexture@CDisplayManager@@QAEXAAVCTexture@@ABVCSurface@@@Z':0x9bf350,  # derived from retail site; not in symbol_index
 '__CxxThrowException@8':0xbfeb84,  # derived from retail site; not in symbol_index
 '__TI2?AVCGenericException@@':0x13692f8,
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_SmallQueries_009be380.cpp'
ROOTS=[
 dict(name='interval_time',source=F,symbol='?CalcPresentationIntervalTime@CDisplayManager@@AAEMJJ@Z',address=0x9be380,size=11,instructions=3,relocations=0),
 dict(name='flush_video',source=F,symbol='?FlushVideoMemeory@CDisplayManager@@QAEXXZ',address=0x9be400,size=10,instructions=5,relocations=0),
 dict(name='texture_memory',source=F,symbol='?GetAvailableTextureMemory@CDisplayManager@@QAEJXZ',address=0x9be410,size=10,instructions=5,relocations=0),
 dict(name='tv_area',source=F,symbol='?GetAVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ',address=0x9becb0,size=38,instructions=8,relocations=0),
 dict(name='vs_version',source=F,symbol='?GetVertexShaderVersion@CDisplayManager@@QBEKXZ',address=0x9bede0,size=29,instructions=7,relocations=0),
 dict(name='refresh_rate',source=F,symbol='?ReadDeviceRefreshRate@CDisplayManager@@AAEJXZ',address=0x9be340,size=54,instructions=19,relocations=2),
 dict(name='copy_back_buffer',source=F,symbol='?CopyBackBufferToTexture@CDisplayManager@@QAEAAVCTexture@@XZ',address=0x9bf5e0,size=18,instructions=9,relocations=1),
 dict(name='interval_lock',source=F,symbol='?ChangePresentationIntervalLockToGivenFrameRateOrLower@CDisplayManager@@QAEXM_N@Z',address=0x9be330,size=3,instructions=1,relocations=0),
 dict(name='release_tile',source=F,symbol='?ReleaseTile@CDisplayManager@@QAEXJAAVCSurface@@@Z',address=0x9bee20,size=3,instructions=1,relocations=0),
 dict(name='alloc_tile',source=F,symbol='?AllocTile@CDisplayManager@@QAEJAAVCSurface@@_N@Z',address=0x9bee30,size=5,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-small-queries.json',[],
  'CDisplayManager small device/state queries and PC no-op tile/interval hooks')
 print('NATIVE_DISPLAY_SMALL_QUERIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
