"""Fresh VC71 parity gate: C3DMeshPartitionElement destructor (0x00AC2100), BuildSubElement (0x00AC2160), ReadFromFile (0x00AC21E0), WriteToFile (0x00AC27A0), Sizeof (0x00AC2930), scalar deleting destructor (0x00AC37B0), C3DMeshFilePartitionChunk WriteToFile (0x00AC2920) / ReadFromFile (0x00AC2970), CCountedPointer<C3DMeshPartitionElement> DeleteData (0x00AC3810) / Reset (0x00AC38A0), CCountedPointer<CArray<C3DTriangle2> > DeleteData (0x00AC3A10) / Reset (0x00AC3A40), C3DMeshPartitionBuilderElement destructor (0x00AC2A30) / BuildOutput (0x00AC2A80), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1C3DMeshPartitionBuilderElement@@QAE@XZ':0xac2a30,
 '??1C3DMeshPartitionElement@@QAE@XZ':0xac2100,
 '??2@YAPAXI@Z':0xbfea1a,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_GC3DMeshPartitionElement@@QAEPAXI@Z':0xac37b0,
 '?BuildOutput@C3DMeshPartitionBuilderElement@@AAEXPAVC3DMeshPartitionElement@@PAV?$CArray@VC3DTriangle2@@@@@Z':0xac2a80,
 '?BuildSubElement@C3DMeshPartitionElement@@QAEPAV1@J@Z':0xac2160,
 '?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@CIXPAX@Z':0xac3a10,
 '?DeleteData@?$CCountedPointer@VC3DMeshPartitionElement@@@@CIXPAX@Z':0xac3810,
 '?ReadFromFile@C3DMeshPartitionElement@@QAEXAAVCDataInputStream@@@Z':0xac21e0,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?Reset@?$CCountedPointer@VC3DMeshPartitionElement@@@@QAEXPAVC3DMeshPartitionElement@@@Z':0xac38a0,
 '?Sizeof@C3DMeshPartitionElement@@QBEKXZ':0xac2930,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteToFile@C3DMeshPartitionElement@@QBEXAAVCDataOutputStream@@@Z':0xac27a0,
 '?_M_insert_overflow@?$vector@VC3DTriangle2@@V?$allocator@VC3DTriangle2@@@_STL@@@_STL@@QAEXPAVC3DTriangle2@@ABV3@ABU__false_type@2@I_N@Z':0xa71550,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='partition_element_destructor',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='??1C3DMeshPartitionElement@@QAE@XZ',address=0xac2100,size=63,instructions=27,relocations=4),
 dict(name='partition_build_sub_element',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?BuildSubElement@C3DMeshPartitionElement@@QAEPAV1@J@Z',address=0xac2160,size=125,instructions=51,relocations=4),
 dict(name='partition_element_read',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?ReadFromFile@C3DMeshPartitionElement@@QAEXAAVCDataInputStream@@@Z',address=0xac21e0,size=1460,instructions=532,relocations=23),
 dict(name='partition_element_write',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?WriteToFile@C3DMeshPartitionElement@@QBEXAAVCDataOutputStream@@@Z',address=0xac27a0,size=374,instructions=135,relocations=27),
 dict(name='partition_chunk_write',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?WriteToFile@C3DMeshFilePartitionChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xac2920,size=16,instructions=5,relocations=1),
 dict(name='partition_element_sizeof',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?Sizeof@C3DMeshPartitionElement@@QBEKXZ',address=0xac2930,size=58,instructions=25,relocations=2),
 dict(name='partition_chunk_read',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?ReadFromFile@C3DMeshFilePartitionChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xac2970,size=76,instructions=30,relocations=3),
 dict(name='partition_element_sdd',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='??_GC3DMeshPartitionElement@@QAEPAXI@Z',address=0xac37b0,size=67,instructions=22,relocations=3),
 dict(name='partition_delete_data',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DMeshPartitionElement@@@@CIXPAX@Z',address=0xac3810,size=60,instructions=21,relocations=3),
 dict(name='partition_reset',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?Reset@?$CCountedPointer@VC3DMeshPartitionElement@@@@QAEXPAVC3DMeshPartitionElement@@@Z',address=0xac38a0,size=103,instructions=36,relocations=3),
 dict(name='partition_builder_destructor',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionBuilderElement_00ac2a30.cpp',symbol='??1C3DMeshPartitionBuilderElement@@QAE@XZ',address=0xac2a30,size=69,instructions=28,relocations=5),
 dict(name='partition_builder_build_output',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionBuilderElement_00ac2a30.cpp',symbol='?BuildOutput@C3DMeshPartitionBuilderElement@@AAEXPAVC3DMeshPartitionElement@@PAV?$CArray@VC3DTriangle2@@@@@Z',address=0xac2a80,size=337,instructions=131,relocations=4),
 dict(name='partition_triangles_delete_data',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@CIXPAX@Z',address=0xac3a10,size=33,instructions=15,relocations=2),
 dict(name='partition_triangles_reset',source=R/'rebuild/src/compiled/00/ac/C3DMeshPartitionElement_Methods_00ac2160.cpp',symbol='?Reset@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@QAEXPAV?$CArray@VC3DTriangle2@@@@@Z',address=0xac3a40,size=103,instructions=36,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-partition-element.json',[],
  'C3DMeshPartitionElement destructor (0x00AC2100), BuildSubElement (0x00AC2160), ReadFromFile (0x00AC21E0), WriteToFile (0')
 print('NATIVE_MESH_PARTITION_ELEMENT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
