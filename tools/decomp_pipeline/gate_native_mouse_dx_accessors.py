"""Fresh VC71 parity gate: CMouseDX small overrides: GetPos 0x00AB46E0, PeekPos/SetInputOn/recentring/cursor/filtering/button states 0x00AB5F00..0x00AB5F90, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='cmousedx_getpos',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?GetPos@CMouseDX@@UAEXPAVC2DVector@@@Z',address=0xab46e0,size=24,instructions=6,relocations=0),
 dict(name='cmousedx_peekpos',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?PeekPos@CMouseDX@@UBEABVC2DVector@@XZ',address=0xab5f00,size=7,instructions=2,relocations=0),
 dict(name='cmousedx_setinputon',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?SetInputOn@CMouseDX@@UAEX_N@Z',address=0xab5f10,size=13,instructions=3,relocations=0),
 dict(name='cmousedx_setrecentringon',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?SetRecentringOn@CMouseDX@@UAEX_N@Z',address=0xab5f20,size=13,instructions=3,relocations=0),
 dict(name='cmousedx_isrecentringon',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?IsRecentringOn@CMouseDX@@UBE_NXZ',address=0xab5f30,size=7,instructions=2,relocations=0),
 dict(name='cmousedx_setdrawcursor',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?SetDrawCursor@CMouseDX@@UAEX_N@Z',address=0xab5f40,size=13,instructions=3,relocations=0),
 dict(name='cmousedx_isdrawingcursor',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?IsDrawingCursor@CMouseDX@@UBE_NXZ',address=0xab5f50,size=7,instructions=2,relocations=0),
 dict(name='cmousedx_setmousefiltering',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?SetMouseFiltering@CMouseDX@@UAEX_N@Z',address=0xab5f60,size=13,instructions=3,relocations=0),
 dict(name='cmousedx_islmbdown',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?IsLMBDown@CMouseDX@@UBE_NXZ',address=0xab5f70,size=7,instructions=2,relocations=0),
 dict(name='cmousedx_ismmbdown',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?IsMMBDown@CMouseDX@@UBE_NXZ',address=0xab5f80,size=7,instructions=2,relocations=0),
 dict(name='cmousedx_isrmbdown',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Accessors_00ab5f00.cpp',symbol='?IsRMBDown@CMouseDX@@UBE_NXZ',address=0xab5f90,size=7,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mouse-dx-accessors.json',[],
  'CMouseDX small overrides: GetPos 0x00AB46E0, PeekPos/SetInputOn/recentring/cursor/filtering/button states 0x00AB5F00..0x')
 print('NATIVE_MOUSE_DX_ACCESSORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
