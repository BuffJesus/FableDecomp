"""Fresh VC71 parity gate: CASuspendableProcess constructor, SuspendableProcess hook, scalar deleting destructor and TerminateProcess (0xA445D0..0xA44A3C, 0xA4B200). Hand-owned fable_suspendable_process.h; journal 2026-10-29."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CASuspendableProcess@@6B@':0x129d428,
}
ROOTS=[
 dict(name='casuspendableprocess_ctor',source=R/'rebuild/src/compiled/00/a4/CASuspendableProcess_Construct_00a445d0.cpp',symbol='??0CASuspendableProcess@@QAE@_NJM@Z',address=0xa445d0,size=42,instructions=14,relocations=2),
 dict(name='casuspendableprocess_suspendableprocess',source=R/'rebuild/src/compiled/00/a4/CASuspendableProcess_Construct_00a445d0.cpp',symbol='?SuspendableProcess@CASuspendableProcess@@UAEXXZ',address=0xa44600,size=1,instructions=1,relocations=0),
 dict(name='casuspendableprocess_sdd',source=R/'rebuild/src/compiled/00/a4/CASuspendableProcess_Construct_00a445d0.cpp',symbol='??_GCASuspendableProcess@@UAEPAXI@Z',address=0xa44a20,size=30,instructions=11,relocations=2),
 dict(name='casuspendableprocess_terminateprocess',source=R/'rebuild/src/compiled/00/a4/CASuspendableProcess_TerminateProcess_00a4b200.cpp',symbol='?TerminateProcess@CASuspendableProcess@@QAEXXZ',address=0xa4b200,size=32,instructions=15,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-suspendable-process.json',[],
  'CASuspendableProcess constructor, SuspendableProcess hook, scalar deleting destructor and TerminateProcess (0xA445D0..0x')
 print('NATIVE_SUSPENDABLE_PROCESS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
