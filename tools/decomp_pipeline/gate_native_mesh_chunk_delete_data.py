"""Fresh VC71 parity gate: CCountedPointer<chunk>::DeleteData instantiations for the mesh-file chunks (0x00A9CB20, 0x00A9CB50, 0x00AC1460, 0x00ACF3B0..0x00ACF440), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='deletedata_c3dmeshfilegroupchunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFileGroupChunk@@@@CIXPAX@Z',address=0xacf3b0,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfileverticeschunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFileVerticesChunk@@@@CIXPAX@Z',address=0xacf420,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfiletriangleschunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFileTrianglesChunk@@@@CIXPAX@Z',address=0xacf430,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfileprimitivechunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFilePrimitiveChunk@@@@CIXPAX@Z',address=0xacf440,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfilerootchunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFileRootChunk@@@@CIXPAX@Z',address=0xa9cb20,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfilemapchunk',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@CIXPAX@Z',address=0xac1460,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3dmeshfile',source=R/'rebuild/src/compiled/00/ac/CCountedPointer_ChunkDeleteData_00acf3b0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshFile@@@@CIXPAX@Z',address=0xa9cb50,size=11,instructions=6,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-chunk-delete-data.json',[],
  'CCountedPointer<chunk>::DeleteData instantiations for the mesh-file chunks (0x00A9CB20, 0x00A9CB50, 0x00AC1460, 0x00ACF3')
 print('NATIVE_MESH_CHUNK_DELETE_DATA_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
