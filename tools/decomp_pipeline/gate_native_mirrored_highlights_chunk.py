"""Fresh VC71 parity gate: CMirroredTextureHighlightsChunk methods (lib_mirrored_texture_highlights_chunk.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXIABK@Z':0x464931,
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':0x464ace,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_04JDCAIGMJ@MTEH?$AA@':0x129df68,  # derived from retail site; not in symbol_index
 '??_7CMirroredTextureHighlightsChunk@@6B@':0x129e0e0,  # derived from retail site; not in symbol_index
 '??_C@_0BI@MDIAGNAH@Number?5of?5verticies?3?5?$CFu?$AA@':0x129ef9c,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ac/CMirroredTextureHighlightsChunk_Methods_00ac4ec0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='get_chunk_info',source=F,symbol='?GetChunkInfo@CMirroredTextureHighlightsChunk@@UBE?BVCCharString@@XZ',address=0xac4ec0,size=92,instructions=25,relocations=5,flags=GS),
 dict(name='get_count',source=F,symbol='?GetCount@CMirroredTextureHighlightsChunk@@QBEKXZ',address=0xac4f20,size=10,instructions=4,relocations=0,flags=GS),
 dict(name='get_list_const',source=F,symbol='?GetList@CMirroredTextureHighlightsChunk@@QBEPBKXZ',address=0xac4f30,size=20,instructions=9,relocations=0,flags=GS),
 dict(name='write_to_file',source=F,symbol='?WriteToFile@CMirroredTextureHighlightsChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xac4f50,size=59,instructions=28,relocations=2,flags=GS),
 dict(name='get_list',source=F,symbol='?GetList@CMirroredTextureHighlightsChunk@@QAEPAKXZ',address=0xac4f90,size=20,instructions=9,relocations=0,flags=GS),
 dict(name='clear_list',source=F,symbol='?ClearList@CMirroredTextureHighlightsChunk@@QAEXXZ',address=0xac4fb0,size=42,instructions=22,relocations=1,flags=GS),
 dict(name='create_new_instance',source=F,symbol='?CreateNewInstance@CMirroredTextureHighlightsChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac4fe0,size=83,instructions=31,relocations=4,flags=GS),
 dict(name='add_vertex_index',source=F,symbol='?AddVertexIndex@CMirroredTextureHighlightsChunk@@QAEXABK@Z',address=0xac5040,size=55,instructions=21,relocations=1,flags=GS),
 dict(name='read_from_file',source=F,symbol='?ReadFromFile@CMirroredTextureHighlightsChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xac5080,size=233,instructions=91,relocations=3,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mirrored-highlights-chunk.json',[],
  'CMirroredTextureHighlightsChunk methods (lib_mirrored_texture_highlights_chunk.cpp)')
 print('NATIVE_MIRRORED_HIGHLIGHTS_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
