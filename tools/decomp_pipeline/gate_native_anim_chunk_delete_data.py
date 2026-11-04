"""Fresh VC71 parity gate: CCountedPointer<T>::DeleteData for the anim-file chunks (0x00A99FD0..0x00A9A000), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='deletedata_cchunkedfilesuperchunk',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_AnimChunkDeleteData_00a99fd0.cpp',symbol='?DeleteData@?$CCountedPointer@VCChunkedFileSuperChunk@@@@CIXPAX@Z',address=0xa99fd0,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3danimfilerootchunk',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_AnimChunkDeleteData_00a99fd0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DAnimFileRootChunk@@@@CIXPAX@Z',address=0xa99fe0,size=11,instructions=6,relocations=0),
 dict(name='deletedata_cchunkedfilechunk',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_AnimChunkDeleteData_00a99fd0.cpp',symbol='?DeleteData@?$CCountedPointer@VCChunkedFileChunk@@@@CIXPAX@Z',address=0xa99ff0,size=11,instructions=6,relocations=0),
 dict(name='deletedata_c3danimfile',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_AnimChunkDeleteData_00a99fd0.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DAnimFile@@@@CIXPAX@Z',address=0xa9a000,size=11,instructions=6,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-chunk-delete-data.json',[],
  'CCountedPointer<T>::DeleteData for the anim-file chunks (0x00A99FD0..0x00A9A000), exact after relocation')
 print('NATIVE_ANIM_CHUNK_DELETE_DATA_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
