"""Fresh VC71 parity gate: C3DMeshFileClothChunk accessors and deleting dtor (lib_3d_mesh_file_cloth.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??1C3DMeshFileClothChunk@@UAE@XZ':0xa9d3c0,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/ac/C3DMeshFileClothChunk_Methods_00ac6600.cpp'
ROOTS=[
 dict(name='get_chunk_info',source=F,symbol='?GetChunkInfo@C3DMeshFileClothChunk@@UBE?BVCCharString@@XZ',address=0xac62b0,size=25,instructions=9,relocations=2),
 dict(name='get_name',source=F,symbol='?GetName@C3DMeshFileClothChunk@@QBE?BVCCharString@@XZ',address=0xac62d0,size=22,instructions=9,relocations=1),
 dict(name='set_name',source=F,symbol='?SetName@C3DMeshFileClothChunk@@QAEXABVCCharString@@@Z',address=0xac62f0,size=8,instructions=2,relocations=1),
 dict(name='set_particle_code',source=F,symbol='?SetParticleCode@C3DMeshFileClothChunk@@QAEXABVCCharString@@@Z',address=0xac6300,size=8,instructions=2,relocations=1),
 dict(name='set_material_index',source=F,symbol='?SetMaterialIndex@C3DMeshFileClothChunk@@QAEXJ@Z',address=0xac6310,size=10,instructions=3,relocations=0),
 dict(name='deleting_dtor',source=F,symbol='??_GC3DMeshFileClothChunk@@UAEPAXI@Z',address=0xac69d0,size=30,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-cloth-chunk-accessors.json',[],
  'C3DMeshFileClothChunk accessors and deleting dtor (lib_3d_mesh_file_cloth.cpp)')
 print('NATIVE_CLOTH_CHUNK_ACCESSORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
