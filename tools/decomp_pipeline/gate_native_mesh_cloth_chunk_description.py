"""Fresh VC71 parity gate: C3DMeshFileClothChunk::GetChunkDescription 0x00AC69B0 ("Cloth"), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??_C@_05GKKHKKE@Cloth?$AA@':0x129f034,
}
ROOTS=[
 dict(name='c3dmeshfileclothchunk_getchunkdescription',source=R/'rebuild/src/compiled/00/ac/C3DMeshFileClothChunk_GetChunkDescriptionC3DMeshFileClothChunkUBEBVCCh_00ac69b0.cpp',symbol='?GetChunkDescription@C3DMeshFileClothChunk@@UBE?BVCCharString@@XZ',address=0xac69b0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-cloth-chunk-description.json',[],
  'C3DMeshFileClothChunk::GetChunkDescription 0x00AC69B0 ("Cloth"), exact after relocation')
 print('NATIVE_MESH_CLOTH_CHUNK_DESCRIPTION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
