"""Fresh VC71 parity gate: C3DMeshFileMaterialExtChunk info/write/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04CJFPMDAL@MTLE?$AA@':0x129edcc,  # derived from retail site; not in symbol_index
 '??_C@_0BE@KAALHIAP@Self?5Illumination?3?5?$AA@':0x129ee1c,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileMaterialExtChunk@@6B@':0x129ee90,  # derived from retail site; not in symbol_index
 '??_C@_0BN@FCPECLD@Extended?5Material?5Properties?$AA@':0x129eebc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialExtChunk_Methods_00abfd70.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileMaterialExtChunk@@UBE?BVCCharString@@XZ',address=0xabfd70,size=73,instructions=22,relocations=6),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileMaterialExtChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabfde0,size=33,instructions=14,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileMaterialExtChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac03b0,size=62,instructions=28,relocations=4),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileMaterialExtChunk@@UBE?BVCCharString@@XZ',address=0xac1920,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-material-ext-chunk.json',[],
  'C3DMeshFileMaterialExtChunk info/write/create/description')
 print('NATIVE_MESH_MATERIAL_EXT_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
