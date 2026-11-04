"""Fresh VC71 parity gate: CResourceList debug statistics and CResourceBank::RemoveResource (ResourceList.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':0x9fc020,
}
F0=R/'rebuild/src/compiled/00/9f/CResource_GetUnusedFrames_009fbf70.cpp'
F1=R/'rebuild/src/compiled/00/9f/CResourceBank_RemoveResource_009fc3c0.cpp'
ROOTS=[
 dict(name='list_debug_memory_in_use',source=F0,symbol='?GetDebugMemoryInUse@CResourceList@@QBEKK@Z',address=0x9fc0b0,size=75,instructions=37,relocations=0),
 dict(name='list_reset_debug_stats',source=F0,symbol='?ResetDebugStats@CResourceList@@QAEXXZ',address=0x9fc100,size=50,instructions=27,relocations=0),
 dict(name='list_frames_since_reset_debug_stats',source=F0,symbol='?GetFramesSinceResetDebugStats@CResourceList@@QBEKXZ',address=0x9fc140,size=7,instructions=3,relocations=0),
 dict(name='list_validate',source=F0,symbol='?Validate@CResourceList@@QAEXXZ',address=0x9fc360,size=24,instructions=9,relocations=0),
 dict(name='bank_remove_resource',source=F1,symbol='?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z',address=0x9fc3c0,size=8,instructions=2,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-resource-list-debug.json',[],
  'CResourceList debug statistics and CResourceBank::RemoveResource (ResourceList.cpp)')
 print('NATIVE_RESOURCE_LIST_DEBUG_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
