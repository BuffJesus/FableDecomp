"""Fresh VC71 parity gate: CParticleSimulation::WriteVectorArray (0x00AE18C0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@_STL@@@_STL@@QAE@I@Z':0x7c2ab0,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='write_vector_array',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_WriteVectorArray_00ae18c0.cpp',symbol='?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z',address=0xae18c0,size=436,instructions=142,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-write-vector-array.json',[],
  'CParticleSimulation::WriteVectorArray (0x00AE18C0), exact after relocation')
 print('NATIVE_PARTICLE_WRITE_VECTOR_ARRAY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
