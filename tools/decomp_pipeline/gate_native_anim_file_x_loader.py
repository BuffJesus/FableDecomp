"""Fresh VC71 parity gate: C3DAnimFileXLoader ctor and deleting dtor (lib_3d_anim_file_x.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RegisterObjectSubChunkType@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa98c80,
 '?RegisterHelpersSubChunkType@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa98d20,
 '?RegisterMovementVectorSubChunkType@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa98dc0,
 '??1C3DAnimFileLoader@@UAE@XZ':0xa996d0,
 '??0C3DAnimFileLoader@@QAE@XZ':0xa999b0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??1CChunkedFileChunk@@UAE@XZ':0xaa75c0,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_C@_04DNPHAPJK@XSEQ?$AA@':0x129d4f8,  # derived from retail site; not in symbol_index
 '??_C@_04BMHHJENO@XALO?$AA@':0x129d500,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileXAllocationSizeChunk@@6B@':0x129e144,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileXSequenceChunk@@6B@':0x129e194,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileXLoader@@6B@':0x129e1e4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DAnimFileXSequenceChunk_Methods_00aa4650.cpp'
ROOTS=[
 dict(name='ctor',source=F,symbol='??0C3DAnimFileXLoader@@QAE@XZ',address=0xaa4710,size=365,instructions=111,relocations=22),
 dict(name='deleting_dtor',source=F,symbol='??_GC3DAnimFileXLoader@@UAEPAXI@Z',address=0xaa4a20,size=30,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-file-x-loader.json',[],
  'C3DAnimFileXLoader ctor and deleting dtor (lib_3d_anim_file_x.cpp)')
 print('NATIVE_ANIM_FILE_X_LOADER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
