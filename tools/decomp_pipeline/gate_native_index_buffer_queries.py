"""Fresh VC71 parity gate: CIndexBuffer Unlock (0x00AC1F00), GetSize (0x00AC1F10), Sizeof (0x00AC1F30) and the scalar deleting destructor (0x00AC2070, inline ~CIndexBuffer, emitted with the constructors), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CIndexBuffer@@6B@':0x129ef2c,
 '??_7CMovableResource@@6B@':0x129b860,
 '?Uninitialise@CIndexBuffer@@QAEXXZ':0xac1ee0,
}
ROOTS=[
 dict(name='index_buffer_unlock',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Queries_00ac1f00.cpp',symbol='?Unlock@CIndexBuffer@@QAEXXZ',address=0xac1f00,size=10,instructions=5,relocations=0),
 dict(name='index_buffer_get_size',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Queries_00ac1f00.cpp',symbol='?GetSize@CIndexBuffer@@QBEJXZ',address=0xac1f10,size=24,instructions=10,relocations=0),
 dict(name='index_buffer_sizeof',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Queries_00ac1f00.cpp',symbol='?Sizeof@CIndexBuffer@@QBEKXZ',address=0xac1f30,size=27,instructions=11,relocations=0),
 dict(name='index_buffer_sdd',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Construction_00ac1ec0.cpp',symbol='??_GCIndexBuffer@@UAEPAXI@Z',address=0xac2070,size=42,instructions=13,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-index-buffer-queries.json',[],
  'CIndexBuffer Unlock (0x00AC1F00), GetSize (0x00AC1F10), Sizeof (0x00AC1F30) and the scalar deleting destructor (0x00AC20')
 print('NATIVE_INDEX_BUFFER_QUERIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
