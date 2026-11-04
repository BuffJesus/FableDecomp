"""Fresh VC71 parity gate: GFGet2DBoxDistance / GFGet3DBoxDistance, float and integer forms (largest per-axis distance; 3D takes Z first)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/9e/Global_GFBoxDistance_009e1ad0.cpp'
ROOTS=[
 dict(name='box2_vector',source=F,symbol='?GFGet2DBoxDistance@@YIMABVC2DVector@@0@Z',address=0x9e1ad0,size=35,instructions=16,relocations=0),
 dict(name='box2_coord',source=F,symbol='?GFGet2DBoxDistance@@YIJABVC2DCoordI@@0@Z',address=0x9e1b00,size=35,instructions=20,relocations=0),
 dict(name='box3_vector',source=F,symbol='?GFGet3DBoxDistance@@YIMABVC3DVector@@0@Z',address=0x9e1b30,size=68,instructions=26,relocations=0),
 dict(name='box3_coord',source=F,symbol='?GFGet3DBoxDistance@@YIJABVC3DCoordI@@0@Z',address=0x9e1b80,size=56,instructions=31,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-box-distance.json',[],
  'GFGet2DBoxDistance / GFGet3DBoxDistance, float and integer forms (largest per-axis distance; 3D takes Z first)')
 print('NATIVE_GLOBAL_BOX_DISTANCE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
