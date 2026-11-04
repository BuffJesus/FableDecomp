"""Fresh VC71 parity gate: CIndexBuffer Uninitialise/RelocateData/OnPostRelocation (0x00AC1EE0..0x00AC1F60) and the CVertexStreamConverter ctor 0x00AC44C0, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='uninitialise',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Uninitialise_00ac1ee0.cpp',symbol='?Uninitialise@CIndexBuffer@@QAEXXZ',address=0xac1ee0,size=25,instructions=11,relocations=0),
 dict(name='relocate_data',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Relocation_00ac1f50.cpp',symbol='?RelocateData@CIndexBuffer@@UAEXJPAX0J@Z',address=0xac1f50,size=3,instructions=1,relocations=0),
 dict(name='on_post_relocation',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Relocation_00ac1f50.cpp',symbol='?OnPostRelocation@CIndexBuffer@@QAEXXZ',address=0xac1f60,size=1,instructions=1,relocations=0),
 dict(name='vertex_stream_converter_ctor',source=R/'rebuild/src/compiled/00/ac/CVertexStreamConverter_CVertexStreamConverter_00ac44c0.cpp',symbol='??0CVertexStreamConverter@@QAE@XZ',address=0xac44c0,size=3,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-index-buffer-relocation.json',[],
  'CIndexBuffer Uninitialise/RelocateData/OnPostRelocation (0x00AC1EE0..0x00AC1F60) and the CVertexStreamConverter ctor 0x0')
 print('NATIVE_INDEX_BUFFER_RELOCATION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
