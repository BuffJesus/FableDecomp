"""Fresh VC71 parity gate: QuadricOptimiserInternals::COptimisedPrimitive::FindFaceTextureTangents (0x00AC7E10), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FindTextureTangent@COptimisedPrimitive@QuadricOptimiserInternals@@QAE?AVC3DVector@@ABV3@00MMM@Z':0xac7300,
}
ROOTS=[
 dict(name='optimised_face_texture_tangents',source=R/'rebuild/src/compiled/00/ac/COptimisedPrimitive_FindFaceTextureTangents_00ac7e10.cpp',symbol='?FindFaceTextureTangents@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXAAVCOptimisedFace@2@@Z',address=0xac7e10,size=155,instructions=62,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-optimised-face-texture-tangents.json',[],
  'QuadricOptimiserInternals::COptimisedPrimitive::FindFaceTextureTangents (0x00AC7E10), exact after relocation')
 print('NATIVE_OPTIMISED_FACE_TEXTURE_TANGENTS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
