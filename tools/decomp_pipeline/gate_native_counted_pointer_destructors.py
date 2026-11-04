"""Fresh VC71 parity gate: Out-of-line CCountedPointer<T> destructors 0x00A8E000, 0x00A94C70, 0x00A9A350, 0x00AAC870, 0x00A9A3E0, 0x00A9CE90, 0x00AC1A90, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??3@YAXPAX@Z':0xbfe9bc,
}
ROOTS=[
 dict(name='ccountedpointer_dtor_0xa8e000',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VCTriangleStripifierBase@@@@QAE@XZ',address=0xa8e000,size=53,instructions=19,relocations=1),
 dict(name='ccountedpointer_dtor_0xa94c70',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VC3DMeshPartitionElement@@@@QAE@XZ',address=0xa94c70,size=53,instructions=19,relocations=1),
 dict(name='ccountedpointer_dtor_0xa9a350',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VCChunkedFileSuperChunk@@@@QAE@XZ',address=0xa9a350,size=53,instructions=19,relocations=1),
 dict(name='ccountedpointer_dtor_0xaac870',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@QAE@XZ',address=0xaac870,size=53,instructions=19,relocations=1),
 dict(name='dtor_c3danimfilerootchunk',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VC3DAnimFileRootChunk@@@@QAE@XZ',address=0xa9a3e0,size=53,instructions=19,relocations=1),
 dict(name='dtor_c3dmeshfilerootchunk',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VC3DMeshFileRootChunk@@@@QAE@XZ',address=0xa9ce90,size=53,instructions=19,relocations=1),
 dict(name='dtor_c3dmeshfilemapchunk',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_Destructors_00a8e000.cpp',symbol='??1?$CCountedPointer@VC3DMeshFileMapChunk@@@@QAE@XZ',address=0xac1a90,size=53,instructions=19,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-counted-pointer-destructors.json',[],
  'Out-of-line CCountedPointer<T> destructors 0x00A8E000, 0x00A94C70, 0x00A9A350, 0x00AAC870, exact after relocation')
 print('NATIVE_COUNTED_POINTER_DESTRUCTORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
