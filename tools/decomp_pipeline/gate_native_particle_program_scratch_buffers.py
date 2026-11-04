"""Fresh VC71 parity gate: CParticleProgram::GetMatrixBuffer (0x00AE04E0), GetVectorBuffer (0x00AE0540), GetGroupVectorBuffer (0x00AE05A0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Allocate@CSimpleMemoryPool@@QAEPAXK@Z':0xa5c910,
 '?FreeAll@CSimpleMemoryPool@@QAEXXZ':0xa5c720,
 '?GroupVectorBuffer@CParticleProgram@@0PAVC4DVector@@A':0x13d2da8,
 '?GroupVectorBufferSize@CParticleProgram@@0KA':0x13d2dac,
 '?MatrixBuffer@CParticleProgram@@0PAVCMatrix4x4@@A':0x13d2d98,
 '?MatrixBufferSize@CParticleProgram@@0KA':0x13d2d9c,
 '?VectorBuffer@CParticleProgram@@0PAVC4DVector@@A':0x13d2da0,
 '?VectorBufferSize@CParticleProgram@@0KA':0x13d2da4,
 '_GroupVectorBufferPool':0x13d2de8,
 '_MatrixBufferPool':0x13d2db0,
 '_VectorBufferPool':0x13d2dcc,
}
ROOTS=[
 dict(name='matrix_buffer',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ScratchBuffers_00ae04e0.cpp',symbol='?GetMatrixBuffer@CParticleProgram@@CIPAVCMatrix4x4@@K@Z',address=0xae04e0,size=95,instructions=33,relocations=10),
 dict(name='vector_buffer',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ScratchBuffers_00ae04e0.cpp',symbol='?GetVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z',address=0xae0540,size=95,instructions=33,relocations=10),
 dict(name='group_vector_buffer',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ScratchBuffers_00ae04e0.cpp',symbol='?GetGroupVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z',address=0xae05a0,size=95,instructions=33,relocations=10),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-scratch-buffers.json',[],
  'CParticleProgram::GetMatrixBuffer (0x00AE04E0), GetVectorBuffer (0x00AE0540), GetGroupVectorBuffer (0x00AE05A0), exact a')
 print('NATIVE_PARTICLE_PROGRAM_SCRATCH_BUFFERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
