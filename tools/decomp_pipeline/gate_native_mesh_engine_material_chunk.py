"""Fresh VC71 parity gate: C3DMeshFileEngineMaterialChunk info/ctor/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '??0C3DMeshFileEngineMaterialChunk@@QAE@XZ':0xac56f0,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04PHMJNJFO@EMTL?$AA@':0x129ded4,  # derived from retail site; not in symbol_index
 '??_C@_06GECNACHD@Name?3?5?$AA@':0x129e56c,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileEngineMaterialChunk@@6B@':0x129efc8,  # derived from retail site; not in symbol_index
 '??_C@_0BA@BBAGBBAN@Engine?5Material?$AA@':0x129eff4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ac/C3DMeshFileEngineMaterialChunk_Methods_00ac51d0.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileEngineMaterialChunk@@UBE?BVCCharString@@XZ',address=0xac51d0,size=79,instructions=24,relocations=6),
 dict(name='ctor',source=F,symbol='??0C3DMeshFileEngineMaterialChunk@@QAE@XZ',address=0xac56f0,size=71,instructions=25,relocations=4),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileEngineMaterialChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac6180,size=46,instructions=22,relocations=2),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileEngineMaterialChunk@@UBE?BVCCharString@@XZ',address=0xac6270,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-engine-material-chunk.json',[],
  'C3DMeshFileEngineMaterialChunk info/ctor/create/description')
 print('NATIVE_MESH_ENGINE_MATERIAL_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
