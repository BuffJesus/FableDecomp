"""Fresh VC71 parity gate: CDataInputStream::Read2DVector / Read3DVector (float locals) / Read3DAngles / ReadMatrix3x4 (ReadValue<float> temporaries), all through the inline Read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
}
F=R/'rebuild/src/compiled/00/99/CDataInputStream_VectorReaders_00995030.cpp'
ROOTS=[
 dict(name='read_2d',source=F,symbol='?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ',address=0x995030,size=188,instructions=71,relocations=2),
 dict(name='read_3d',source=F,symbol='?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ',address=0x9950f0,size=273,instructions=102,relocations=3),
 dict(name='read_angles',source=F,symbol='?Read3DAngles@CDataInputStream@@QAE?AVC3DAngles@@XZ',address=0x995210,size=299,instructions=109,relocations=3),
 dict(name='read_matrix',source=F,symbol='?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ',address=0x995340,size=1064,instructions=385,relocations=12),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-input-vectors.json',[],
  'CDataInputStream::Read2DVector / Read3DVector (float locals) / Read3DAngles / ReadMatrix3x4 (ReadValue<float> temporaries), all through the inline Read')
 print('NATIVE_DATA_INPUT_VECTORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
