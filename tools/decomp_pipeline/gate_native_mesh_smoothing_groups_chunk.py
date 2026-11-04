"""Fresh VC71 parity gate: C3DMeshFileSmoothingGroupsChunk write/create/read/description + scalar deleting dtor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':0x464ace,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?reserve@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXI@Z':0x9b4030,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??1CChunkedFileChunk@@UAE@XZ':0xaa75c0,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '??_C@_04HMAFHOHI@SMTH?$AA@':0x129eb04,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileSmoothingGroupsChunk@@6B@':0x129ec28,  # derived from retail site; not in symbol_index
 '??_C@_0BB@HCJPNAPL@Smoothing?5Groups?$AA@':0x129ec54,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileSmoothingGroupsChunk_Methods_00abbfc0.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileSmoothingGroupsChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabbfc0,size=68,instructions=31,relocations=3),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileSmoothingGroupsChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabc500,size=83,instructions=31,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileSmoothingGroupsChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xabc980,size=289,instructions=114,relocations=5),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileSmoothingGroupsChunk@@UBE?BVCCharString@@XZ',address=0xabea90,size=25,instructions=9,relocations=2),
 dict(name='deleting_dtor',source=F,symbol='??_GC3DMeshFileSmoothingGroupsChunk@@UAEPAXI@Z',address=0xabecb0,size=54,instructions=19,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-smoothing-groups-chunk.json',[],
  'C3DMeshFileSmoothingGroupsChunk write/create/read/description + scalar deleting dtor')
 print('NATIVE_MESH_SMOOTHING_GROUPS_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
