"""Fresh VC71 parity gate: Implicit/inline members emitted out of line (engine-material dtor, wave format header ctor)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,
}
F0=R/'rebuild/src/compiled/00/a9/C3DMeshFileLoader_Register_00a9ad00.cpp'
F1=R/'rebuild/src/compiled/00/aa/CWaveFile_Methods_00aad120.cpp'
ROOTS=[
 dict(name='engine_material_dtor',source=F0,symbol='??1C3DMeshFileEngineMaterialChunk@@UAE@XZ',address=0xa9d270,size=43,instructions=15,relocations=2),
 dict(name='wave_format_header_ctor',source=F1,symbol='??0CWaveFormatHeader@CWaveLoader@@QAE@XZ',address=0xaad100,size=8,instructions=3,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-implicit-copies.json',[],
  'Implicit/inline members emitted out of line (engine-material dtor, wave format header ctor)')
 print('NATIVE_IMPLICIT_COPIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
