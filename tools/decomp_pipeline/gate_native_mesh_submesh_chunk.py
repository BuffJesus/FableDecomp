"""Fresh VC71 parity gate: C3DMeshFileSubMeshChunk WriteToFile / CreateNewInstance / ReadFromFile / GetChunkInfo / GetChunkDescription."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '__snprintf':0xbfeb96,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
 '??_C@_04IMHECKIP@SUBM?$AA@':0x129e06c,  # derived from retail site; not in symbol_index
 '??_C@_0DM@PFEJPEPO@Name?3?5?$CFs?0?5Index?3?5?$CFli?0?5Parent?3?5?$CFl@':0x129e940,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileSubMeshChunk@@6B@':0x129eb18,  # derived from retail site; not in symbol_index
 '??_C@_08EGIKOOFN@Sub?9Mesh?$AA@':0x129eb44,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileSubMeshChunk_Methods_00ab9af0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileSubMeshChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xab9af0,size=81,instructions=32,relocations=6,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileSubMeshChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xab9b50,size=85,instructions=35,relocations=5,flags=GS),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileSubMeshChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xab9fc0,size=425,instructions=161,relocations=7,flags=GS),
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ',address=0xaba390,size=128,instructions=38,relocations=6,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ',address=0xabd3c0,size=25,instructions=9,relocations=2,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-submesh-chunk.json',[],
  'C3DMeshFileSubMeshChunk WriteToFile / CreateNewInstance / ReadFromFile / GetChunkInfo / GetChunkDescription')
 print('NATIVE_MESH_SUBMESH_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
