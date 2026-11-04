"""Fresh VC71 parity gate: CVertexGroupBlock basics and the XAPB animated primitive block."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_erase@?$_Rb_tree@KU?$pair@$$CBKJ@_STL@@U?$_Select1st@U?$pair@$$CBKJ@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@_STL@@@2@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBKJ@_STL@@@2@@Z':0x4507a1,
 '?insert_unique@?$_Rb_tree@KU?$pair@$$CBKJ@_STL@@U?$_Select1st@U?$pair@$$CBKJ@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@_STL@@@2@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKJ@_STL@@@_STL@@_N@2@ABU?$pair@$$CBKJ@2@@Z':0x45ca09,
 '?_M_fill_insert@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKIABK@Z':0x4649a3,
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':0x464ace,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?reserve@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXI@Z':0x9b4030,
 '??1CVertexGroupBlock@@QAE@XZ':0xa96da0,
 '?_M_fill_insert@?$vector@VC3DIndexedBoneWeight@@V?$allocator@VC3DIndexedBoneWeight@@@_STL@@@_STL@@QAEXPAVC3DIndexedBoneWeight@@IABV3@@Z':0xa97180,
 '??1C3DMeshFileXAnimatedPrimitiveBlockChunk@@UAE@XZ':0xaa7480,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,
 '??0CVertexGroupBlock@@QAE@XZ':0xad9030,
 '?AddGroup@CVertexGroupBlock@@QAEXK@Z':0xad91f0,
 '??3@YAXPAX@Z':0xbfe9bc,
 '_malloc':0xbfea0e,
 '_free':0xbfea14,
 '??2@YAPAXI@Z':0xbfea1a,
 '__ftol2':0xbfea70,
 '_memmove':0xbfeae6,
 '__real@4f800000':0x122dcb4,
 '??_C@_04LMOFLAIH@XAPB?$AA@':0x129df58,
 '??_7C3DMeshFileXAnimatedPrimitiveBlockChunk@@6B@':0x129e404,
 '??_C@_0BJ@MKONCAPB@Animated?5Primitive?5Block?$AA@':0x129e430,
}
F0=R/'rebuild/src/compiled/00/ad/CVertexGroupBlock_Methods_00ad9030.cpp'
F1=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='vgb_ctor',source=F0,symbol='??0CVertexGroupBlock@@QAE@XZ',address=0xad9030,size=58,instructions=24,relocations=1),
 dict(name='vgb_ctor_capacity',source=F0,symbol='??0CVertexGroupBlock@@QAE@K@Z',address=0xad9070,size=72,instructions=28,relocations=2),
 dict(name='vgb_clear',source=F0,symbol='?Clear@CVertexGroupBlock@@QAEXXZ',address=0xad90c0,size=83,instructions=38,relocations=2),
 dict(name='vgb_add_group',source=F0,symbol='?AddGroup@CVertexGroupBlock@@QAEXK@Z',address=0xad91f0,size=98,instructions=39,relocations=2),
 dict(name='vgb_dtor',source=F0,symbol='??1CVertexGroupBlock@@QAE@XZ',address=0xa96da0,size=84,instructions=34,relocations=3),
 dict(name='xapb_ctor',source=F1,symbol='??0C3DMeshFileXAnimatedPrimitiveBlockChunk@@QAE@XZ',address=0xaa5b90,size=51,instructions=17,relocations=4),
 dict(name='xapb_clear_vertices',source=F1,symbol='?ClearVertices@C3DMeshFileXAnimatedPrimitiveBlockChunk@@QAEXXZ',address=0xaa5bd0,size=80,instructions=37,relocations=1),
 dict(name='xapb_create_new_instance',source=F1,symbol='?CreateNewInstance@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa5c20,size=93,instructions=40,relocations=5),
 dict(name='xapb_deleting_dtor',source=F1,symbol='??_GC3DMeshFileXAnimatedPrimitiveBlockChunk@@UAEPAXI@Z',address=0xaa7460,size=30,instructions=11,relocations=2),
 dict(name='xapb_dtor',source=F1,symbol='??1C3DMeshFileXAnimatedPrimitiveBlockChunk@@UAE@XZ',address=0xaa7480,size=51,instructions=19,relocations=4),
 dict(name='xapb_write_to_file',source=F1,symbol='?WriteToFile@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa5720,size=204,instructions=82,relocations=10),
 dict(name='xapb_read_from_file',source=F1,symbol='?ReadFromFile@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa6c70,size=919,instructions=349,relocations=14),
 dict(name='xapb_description',source=F1,symbol='?GetChunkDescription@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBE?BVCCharString@@XZ',address=0xaa73b0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-vertex-group-block.json',[],
  'CVertexGroupBlock basics and the XAPB animated primitive block')
 print('NATIVE_VERTEX_GROUP_BLOCK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
