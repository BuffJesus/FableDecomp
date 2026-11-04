"""Fresh VC71 parity gate: lib_chunked_file loader/saver (LoadChunkedFile, SaveChunkedFile), CChunkedFile, raw-data chunk, sub-chunk queries and edits."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??1?$CCountedPointer@VCChunkedFileSuperChunk@@@@QAE@XZ':0xa9a350,  # derived from retail site; not in symbol_index
 '??$__copy@PAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV1@H@_STL@@YIPAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z':0xaa8f40,  # derived from retail site; not in symbol_index
 '??$__destroy_aux@PAV?$CCountedPointer@VCChunkedFileChunk@@@@@_STL@@YIXPAV?$CCountedPointer@VCChunkedFileChunk@@@@0ABU__false_type@0@@Z':0xaa9140,  # derived from retail site; not in symbol_index
 '?erase@?$vector@V?$CCountedPointer@VCChunkedFileChunk@@@@V?$allocator@V?$CCountedPointer@VCChunkedFileChunk@@@@@_STL@@@_STL@@QAEPAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV3@@Z':0xaa91f0,  # derived from retail site; not in symbol_index
 '?_M_insert_overflow@?$vector@V?$CCountedPointer@VCChunkedFileChunk@@@@V?$allocator@V?$CCountedPointer@VCChunkedFileChunk@@@@@_STL@@@_STL@@QAEXPAV?$CCountedPointer@VCChunkedFileChunk@@@@ABV3@ABU__false_type@2@I_N@Z':0xaa94e0,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_V@YAXPAX@Z':0xbfeb1c,
 '??_U@YAPAXI@Z':0xbfeb22,
 '__CxxThrowException@8':0xbfeb84,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_04FIAIFCHA@?$DO?$DO?$DO?$DO?$AA@':0x129e454,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileLoader@@6B@':0x129e464,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileSaver@@6B@':0x129e46c,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileChunk@@6B@':0x129e474,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileRawDataChunk@@6B@':0x129e4b8,  # derived from retail site; not in symbol_index
 '??_7CChunkedFile@@6B@':0x129e4e8,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileSuperChunk@@6B@':0x129e4f0,  # derived from retail site; not in symbol_index
 '__TI2?AVCChunkedFileException@@':0x1369548,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/CChunkedFileChunk_Base_00aa75d0.cpp'
ROOTS=[
 dict(name='loader_ctor',source=F,symbol='??0CChunkedFileLoader@@QAE@PBD@Z',address=0xaa7510,size=40,instructions=14,relocations=1),
 dict(name='loader_dtor',source=F,symbol='??1CChunkedFileLoader@@UAE@XZ',address=0xaa7540,size=7,instructions=2,relocations=1),
 dict(name='saver_ctor',source=F,symbol='??0CChunkedFileSaver@@QAE@PBD@Z',address=0xaa7550,size=33,instructions=13,relocations=1),
 dict(name='saver_dtor',source=F,symbol='??1CChunkedFileSaver@@UAE@XZ',address=0xaa7580,size=7,instructions=2,relocations=1),
 dict(name='is_parent_recursive',source=F,symbol='?IsParentRecursive@CChunkedFileChunk@@QBEHPBV1@@Z',address=0xaa7650,size=40,instructions=15,relocations=0),
 dict(name='raw_ctor',source=F,symbol='??0CChunkedFileRawDataChunk@@QAE@PBD@Z',address=0xaa76d0,size=59,instructions=22,relocations=3),
 dict(name='raw_dtor',source=F,symbol='??1CChunkedFileRawDataChunk@@UAE@XZ',address=0xaa7710,size=35,instructions=11,relocations=4),
 dict(name='raw_get_data',source=F,symbol='?GetData@CChunkedFileRawDataChunk@@QBEPAXXZ',address=0xaa7750,size=4,instructions=2,relocations=0),
 dict(name='raw_set_data',source=F,symbol='?SetData@CChunkedFileRawDataChunk@@QAEXPBXK@Z',address=0xaa7760,size=61,instructions=26,relocations=2),
 dict(name='get_no_sub_chunks',source=F,symbol='?GetNoSubChunks@CChunkedFileSuperChunk@@QBEKXZ',address=0xaa78b0,size=10,instructions=4,relocations=0),
 dict(name='find_registered',source=F,symbol='?FindRegisteredSubChunkType@CChunkedFileSuperChunk@@ABEPBVCChunkedFileChunk@@PBD@Z',address=0xaa7950,size=102,instructions=48,relocations=0),
 dict(name='save_chunked_file',source=F,symbol='?SaveChunkedFile@CChunkedFileSaver@@IAEKAAVCChunkedFile@@AAVCDataOutputStream@@@Z',address=0xaa7c70,size=305,instructions=122,relocations=9),
 dict(name='file_ctor',source=F,symbol='??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z',address=0xaa7db0,size=63,instructions=20,relocations=4),
 dict(name='file_dtor',source=F,symbol='??1CChunkedFile@@UAE@XZ',address=0xaa7df0,size=74,instructions=23,relocations=4),
 dict(name='get_sub_chunk_at',source=F,symbol='?GetSubChunkAt@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@K@Z',address=0xaa7e40,size=34,instructions=12,relocations=0),
 dict(name='get_sub_chunk_at_const',source=F,symbol='?GetSubChunkAt@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@K@Z',address=0xaa7e70,size=34,instructions=12,relocations=0),
 dict(name='count_of_type',source=F,symbol='?CountSubChunksOfType@CChunkedFileSuperChunk@@QBEKPBD@Z',address=0xaa7ea0,size=147,instructions=65,relocations=1),
 dict(name='find_first_const',source=F,symbol='?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z',address=0xaa7f40,size=164,instructions=72,relocations=0),
 dict(name='find_next_const',source=F,symbol='?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z',address=0xaa7ff0,size=239,instructions=102,relocations=0),
 dict(name='find_first',source=F,symbol='?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z',address=0xaa80e0,size=164,instructions=72,relocations=0),
 dict(name='find_next',source=F,symbol='?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z',address=0xaa8190,size=239,instructions=102,relocations=0),
 dict(name='remove_sub_chunk',source=F,symbol='?RemoveSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z',address=0xaa8280,size=56,instructions=21,relocations=1),
 dict(name='remove_of_type',source=F,symbol='?RemoveSubChunksOfType@CChunkedFileSuperChunk@@QAEXPBD@Z',address=0xaa82c0,size=147,instructions=63,relocations=1),
 dict(name='super_copy_ctor',source=F,symbol='??0CChunkedFileSuperChunk@@AAE@ABV0@@Z',address=0xaa83b0,size=72,instructions=26,relocations=3),
 dict(name='remove_all',source=F,symbol='?RemoveAllSubChunks@CChunkedFileSuperChunk@@QAEXXZ',address=0xaa8400,size=89,instructions=39,relocations=2),
 dict(name='add_sub_chunk',source=F,symbol='?AddSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z',address=0xaa84f0,size=69,instructions=29,relocations=1),
 dict(name='register_type',source=F,symbol='?RegisterSubChunkType@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z',address=0xaa87e0,size=69,instructions=28,relocations=1),
 dict(name='loader_deleting_dtor',source=F,symbol='??_GCChunkedFileLoader@@UAEPAXI@Z',address=0xaa8d10,size=31,instructions=11,relocations=2),
 dict(name='saver_deleting_dtor',source=F,symbol='??_GCChunkedFileSaver@@UAEPAXI@Z',address=0xaa8d30,size=31,instructions=11,relocations=2),
 dict(name='load_chunked_file',source=F,symbol='?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@AAVCDataInputStream@@@Z',address=0xaa79c0,size=683,instructions=242,relocations=16),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-chunked-file-tree.json',[],
  'lib_chunked_file loader/saver (LoadChunkedFile, SaveChunkedFile), CChunkedFile, raw-data chunk, sub-chunk queries and edits')
 print('NATIVE_CHUNKED_FILE_TREE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
