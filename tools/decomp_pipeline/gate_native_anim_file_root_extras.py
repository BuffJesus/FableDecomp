"""Fresh VC71 parity gate: C3DAnimFileRootChunk ctor/setters and C3DAnimFileSaver."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CChunkedFileSaver@@QAE@PBD@Z':0xaa7550,
 '?SaveChunkedFile@CChunkedFileSaver@@IAEKAAVCChunkedFile@@AAVCDataOutputStream@@@Z':0xaa7c70,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '??_C@_04JGGKDEJL@ANRT?$AA@':0x129dfbc,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileSaver@@6B@':0x129dfe0,  # derived from retail site; not in symbol_index
 '??_C@_04BMFBAKKK@3DAF?$AA@':0x129dfe4,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileRootChunk@@6B@':0x129dff0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a9/C3DAnimFileChunks_00a98a70.cpp'
ROOTS=[
 dict(name='root_ctor',source=F,symbol='??0C3DAnimFileRootChunk@@QAE@XZ',address=0xa98a30,size=23,instructions=8,relocations=3),
 dict(name='set_cyclic',source=F,symbol='?SetCyclic@C3DAnimFileRootChunk@@QAEX_N@Z',address=0xa98a50,size=10,instructions=3,relocations=0),
 dict(name='set_duration',source=F,symbol='?SetDuration@C3DAnimFileRootChunk@@QAEXM@Z',address=0xa98a60,size=10,instructions=3,relocations=0),
 dict(name='saver_ctor',source=F,symbol='??0C3DAnimFileSaver@@QAE@XZ',address=0xa98a00,size=23,instructions=8,relocations=3),
 dict(name='save_anim_file',source=F,symbol='?SaveAnimFile@C3DAnimFileSaver@@QAEKAAVC3DAnimFile@@AAVCDataOutputStream@@@Z',address=0xa98a20,size=5,instructions=1,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-file-root-extras.json',[],
  'C3DAnimFileRootChunk ctor/setters and C3DAnimFileSaver')
 print('NATIVE_ANIM_FILE_ROOT_EXTRAS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
