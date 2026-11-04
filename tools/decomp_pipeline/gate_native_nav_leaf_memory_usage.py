"""Fresh VC71 parity gate: CNavNavigableLeafNode::GetMemoryUsage 0xA76500 after CSmallVector::Size became Ego_r's signed 1-byte count (retail sign-extends it). Journal 2026-10-29."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='cnavnavigable_getmemoryusage',source=R/'rebuild/src/compiled/00/a7/CNavQuadTreeNodes_Virtuals_00a75b40.cpp',symbol='?GetMemoryUsage@CNavNavigableLeafNode@@MBEKAAK000@Z',address=0xa76500,size=26,instructions=7,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-nav-leaf-memory-usage.json',[],
  "CNavNavigableLeafNode::GetMemoryUsage 0xA76500 after CSmallVector::Size became Ego_r's signed 1-byte count (retail sign-")
 print('NATIVE_NAV_LEAF_MEMORY_USAGE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
