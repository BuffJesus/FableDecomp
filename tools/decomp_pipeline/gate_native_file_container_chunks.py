"""Fresh VC71 parity gate: Container chunks (mesh helpers, material list, mesh root, anim helpers): CreateNewInstance + GetChunkDescription."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04GEKNONIJ@HLPR?$AA@':0x129dfcc,  # derived from retail site; not in symbol_index
 '??_C@_04HKMFGHFA@3DRT?$AA@':0x129e064,  # derived from retail site; not in symbol_index
 '??_C@_04DFMHHGNM@MTLS?$AA@':0x129e074,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileRootChunk@@6B@':0x129e090,  # derived from retail site; not in symbol_index
 '??_C@_0N@NHIBFKDO@3D?5Mesh?5File?$AA@':0x129e0bc,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileHelpersChunk@@6B@':0x129e538,  # derived from retail site; not in symbol_index
 '??_C@_07GAFACEFI@Helpers?$AA@':0x129e564,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileHelpersChunk@@6B@':0x129e7c0,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileMaterialListChunk@@6B@':0x129ede0,  # derived from retail site; not in symbol_index
 '??_C@_0O@JOODAEFK@Material?5List?$AA@':0x129ee0c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DFileContainerChunks_00aa9670.cpp'
ROOTS=[
 dict(name='helpers_create',source=F,symbol='?CreateNewInstance@C3DMeshFileHelpersChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa9670,size=62,instructions=28,relocations=4),
 dict(name='helpers_desc',source=F,symbol='?GetChunkDescription@C3DMeshFileHelpersChunk@@UBE?BVCCharString@@XZ',address=0xaaacd0,size=25,instructions=9,relocations=2),
 dict(name='mtls_create',source=F,symbol='?CreateNewInstance@C3DMeshFileMaterialListChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabfbb0,size=62,instructions=28,relocations=4),
 dict(name='mtls_desc',source=F,symbol='?GetChunkDescription@C3DMeshFileMaterialListChunk@@UBE?BVCCharString@@XZ',address=0xac1270,size=25,instructions=9,relocations=2),
 dict(name='anim_helpers_create',source=F,symbol='?CreateNewInstance@C3DAnimFileHelpersChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaae0b0,size=62,instructions=28,relocations=4),
 dict(name='anim_helpers_desc',source=F,symbol='?GetChunkDescription@C3DAnimFileHelpersChunk@@UBE?BVCCharString@@XZ',address=0xaaea90,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-container-chunks.json',[],
  'Container chunks (mesh helpers, material list, mesh root, anim helpers): CreateNewInstance + GetChunkDescription')
 print('NATIVE_FILE_CONTAINER_CHUNKS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
