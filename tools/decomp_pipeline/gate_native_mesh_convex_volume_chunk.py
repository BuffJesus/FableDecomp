"""Fresh VC71 parity gate: C3DMeshFileConvexVolumeChunk info/write/create/read/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?Write3DVector@CDataOutputStream@@QAEXABVC3DVector@@@Z':0x993fd0,
 '?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ':0x9950f0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?erase@?$vector@VCPlane@@V?$allocator@VCPlane@@@_STL@@@_STL@@QAEPAVCPlane@@PAV3@0@Z':0xaab500,  # derived from retail site; not in symbol_index
 '?_M_fill_insert@?$vector@VCPlane@@V?$allocator@VCPlane@@@_STL@@@_STL@@QAEXPAVCPlane@@IABV3@@Z':0xaab570,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04OMACLOLC@HCVL?$AA@':0x129e52c,  # derived from retail site; not in symbol_index
 '??_C@_06GECNACHD@Name?3?5?$AA@':0x129e56c,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileConvexVolumeChunk@@6B@':0x129e634,  # derived from retail site; not in symbol_index
 '??_C@_0O@IJCJNMAB@Convex?5Volume?$AA@':0x129e660,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileConvexVolumeChunk_Methods_00aa9ad0.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileConvexVolumeChunk@@UBE?BVCCharString@@XZ',address=0xaa9ad0,size=51,instructions=18,relocations=4),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileConvexVolumeChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaaa9a0,size=109,instructions=43,relocations=5),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileConvexVolumeChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaaaa60,size=91,instructions=33,relocations=5),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileConvexVolumeChunk@@UBE?BVCCharString@@XZ',address=0xaab6b0,size=25,instructions=9,relocations=2),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileConvexVolumeChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaaab10,size=447,instructions=165,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-convex-volume-chunk.json',[],
  'C3DMeshFileConvexVolumeChunk info/write/create/read/description')
 print('NATIVE_MESH_CONVEX_VOLUME_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
