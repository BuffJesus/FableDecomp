"""Fresh VC71 parity gate: GFGet2DDistance overloads (C2DCoordI integer squares, C2DVector, four longs); inline fsqrt."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/9e/global_GFGet2DDistance_009e2430.cpp'
ROOTS=[
 dict(name='distance_coord',source=F,symbol='?GFGet2DDistance@@YIMABVC2DCoordI@@0@Z',address=0x9e2430,size=33,instructions=15,relocations=0),
 dict(name='distance_vector',source=F,symbol='?GFGet2DDistance@@YIMABVC2DVector@@0@Z',address=0x9e2460,size=27,instructions=13,relocations=0),
 dict(name='distance_longs',source=F,symbol='?GFGet2DDistance@@YIMJJJJ@Z',address=0x9e2480,size=47,instructions=17,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-2d-distance.json',[],
  'GFGet2DDistance overloads (C2DCoordI integer squares, C2DVector, four longs); inline fsqrt')
 print('NATIVE_GLOBAL_2D_DISTANCE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
