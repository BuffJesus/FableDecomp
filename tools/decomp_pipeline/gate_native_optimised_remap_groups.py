"""Fresh VC71 parity gate: QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups (0x00AC99F0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@@Z':0xac97d0,
}
ROOTS=[
 dict(name='optimised_remap_groups',source=R/'rebuild/src/compiled/00/ac/COptimisedPrimitive_RemapGroups_00ac99f0.cpp',symbol='?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@@Z',address=0xac99f0,size=104,instructions=44,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-optimised-remap-groups.json',[],
  'QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups (0x00AC99F0), exact after relocation')
 print('NATIVE_OPTIMISED_REMAP_GROUPS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
