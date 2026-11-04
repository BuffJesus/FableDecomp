"""Fresh VC71 parity gate: CParticleSimulation::ReadVectorArray (0x00AE1730), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@_STL@@@_STL@@QAE@I@Z':0x7c2ab0,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='vector_array',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_ReadVectorArray_00ae1730.cpp',symbol='?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z',address=0xae1730,size=391,instructions=144,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-read-vector-array.json',[],
  'CParticleSimulation::ReadVectorArray (0x00AE1730), exact after relocation')
 print('NATIVE_PARTICLE_READ_VECTOR_ARRAY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
