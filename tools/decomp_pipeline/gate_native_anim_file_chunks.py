"""Fresh VC71 parity gate: C3DAnimFileRootChunk write/create/read/description; C3DAnimFileXAllocationSizeChunk write/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04BMHHJENO@XALO?$AA@':0x129d500,  # derived from retail site; not in symbol_index
 '??_C@_04JGGKDEJL@ANRT?$AA@':0x129dfbc,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileRootChunk@@6B@':0x129dff0,  # derived from retail site; not in symbol_index
 '??_C@_0BB@LNCNCCGL@3D?5Sequence?5File?$AA@':0x129e01c,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileXAllocationSizeChunk@@6B@':0x129e144,  # derived from retail site; not in symbol_index
 '??_C@_0BN@PHOPFGKD@Allocation?5Size?5Helper?5Chunk?$AA@':0x129e170,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a9/C3DAnimFileChunks_00a98a70.cpp'
ROOTS=[
 dict(name='anrt_write',source=F,symbol='?WriteToFile@C3DAnimFileRootChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xa98a70,size=50,instructions=21,relocations=3),
 dict(name='anrt_create',source=F,symbol='?CreateNewInstance@C3DAnimFileRootChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xa98ab0,size=62,instructions=28,relocations=4),
 dict(name='anrt_read',source=F,symbol='?ReadFromFile@C3DAnimFileRootChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xa98af0,size=226,instructions=91,relocations=3),
 dict(name='anrt_desc',source=F,symbol='?GetChunkDescription@C3DAnimFileRootChunk@@UBE?BVCCharString@@XZ',address=0xa99e30,size=25,instructions=9,relocations=2),
 dict(name='xalo_write',source=F,symbol='?WriteToFile@C3DAnimFileXAllocationSizeChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4500,size=33,instructions=14,relocations=2),
 dict(name='xalo_create',source=F,symbol='?CreateNewInstance@C3DAnimFileXAllocationSizeChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa4530,size=62,instructions=28,relocations=4),
 dict(name='xalo_desc',source=F,symbol='?GetChunkDescription@C3DAnimFileXAllocationSizeChunk@@UBE?BVCCharString@@XZ',address=0xaa4970,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-file-chunks.json',[],
  'C3DAnimFileRootChunk write/create/read/description; C3DAnimFileXAllocationSizeChunk write/create/description')
 print('NATIVE_ANIM_FILE_CHUNKS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
