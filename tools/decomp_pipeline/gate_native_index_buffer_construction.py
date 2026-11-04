"""Fresh VC71 parity gate: CIndexBuffer default ctor (0x00AC1EC0), Initialise (0x00AC1F70), Lock (0x00AC1FB0), ctor (0x00AC2010) and ~C3DClothPrimitive (0x00AC3B80), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7C3DClothPrimitive@@6B@':0x129df74,
 '??_7CIndexBuffer@@6B@':0x129ef2c,
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z':0xa0aa70,
}
ROOTS=[
 dict(name='index_buffer_default_ctor',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Construction_00ac1ec0.cpp',symbol='??0CIndexBuffer@@QAE@XZ',address=0xac1ec0,size=17,instructions=6,relocations=1),
 dict(name='index_buffer_initialise',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Construction_00ac1ec0.cpp',symbol='?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z',address=0xac1f70,size=62,instructions=26,relocations=1),
 dict(name='index_buffer_lock',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Construction_00ac1ec0.cpp',symbol='?Lock@CIndexBuffer@@QAEPAEKKK@Z',address=0xac1fb0,size=95,instructions=37,relocations=2),
 dict(name='index_buffer_ctor',source=R/'rebuild/src/compiled/00/ac/CIndexBuffer_Construction_00ac1ec0.cpp',symbol='??0CIndexBuffer@@QAE@KKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z',address=0xac2010,size=83,instructions=30,relocations=2),
 dict(name='cloth_primitive_dtor',source=R/'rebuild/src/compiled/00/ac/C3DClothPrimitive_Destructor_00ac3b80.cpp',symbol='??1C3DClothPrimitive@@UAE@XZ',address=0xac3b80,size=66,instructions=21,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-index-buffer-construction.json',[],
  'CIndexBuffer default ctor (0x00AC1EC0), Initialise (0x00AC1F70), Lock (0x00AC1FB0), ctor (0x00AC2010) and ~C3DClothPrimi')
 print('NATIVE_INDEX_BUFFER_CONSTRUCTION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
