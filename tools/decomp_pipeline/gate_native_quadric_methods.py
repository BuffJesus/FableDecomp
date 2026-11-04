"""Fresh VC71 parity gate: QuadricOptimiserInternals::CQuadric EvaluateError (0x00AC6D50), operator+= (0x00AC6E80), operator*= (0x00AC6EF0), Initialise (0x00AC7050), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='quadric_evaluate_error',source=R/'rebuild/src/compiled/00/ac/CQuadric_Methods_00ac6d50.cpp',symbol='?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBENABVC3DVector@@@Z',address=0xac6d50,size=104,instructions=45,relocations=0),
 dict(name='quadric_add',source=R/'rebuild/src/compiled/00/ac/CQuadric_Methods_00ac6d50.cpp',symbol='??YCQuadric@QuadricOptimiserInternals@@QAEXABV01@@Z',address=0xac6e80,size=103,instructions=35,relocations=0),
 dict(name='quadric_scale',source=R/'rebuild/src/compiled/00/ac/CQuadric_Methods_00ac6d50.cpp',symbol='??XCQuadric@QuadricOptimiserInternals@@QAEXN@Z',address=0xac6ef0,size=91,instructions=34,relocations=0),
 dict(name='quadric_initialise',source=R/'rebuild/src/compiled/00/ac/CQuadric_Methods_00ac6d50.cpp',symbol='?Initialise@CQuadric@QuadricOptimiserInternals@@QAEXABVC3DVector@@0M@Z',address=0xac7050,size=149,instructions=51,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-quadric-methods.json',[],
  'QuadricOptimiserInternals::CQuadric EvaluateError (0x00AC6D50), operator+= (0x00AC6E80), operator*= (0x00AC6EF0), Initia')
 print('NATIVE_QUADRIC_METHODS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
