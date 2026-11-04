"""Fresh VC71 parity gate: CConsoleCommandLine header-inline ctors and scalar deleting dtor as retail emitted them out of line (vtable 0x0129C400 RTTI .?AVCConsoleCommandLine@@)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClass@@QAE@XZ':0x99a2d0,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_7CConsoleCommandLine@@6B@':0x129c400,
}
F=R/'rebuild/src/compiled/00/9e/CConsoleCommandLine_Constructors_009ea2b0.cpp'
ROOTS=[
 dict(name='default_ctor',source=F,symbol='??0CConsoleCommandLine@@QAE@XZ',address=0x9ea2b0,size=41,instructions=14,relocations=4),
 dict(name='copy_ctor',source=F,symbol='??0CConsoleCommandLine@@QAE@ABV0@@Z',address=0x9ea2e0,size=50,instructions=18,relocations=3),
 dict(name='text_ctor',source=F,symbol='??0CConsoleCommandLine@@QAE@ABVCCharString@@@Z',address=0x9eea10,size=61,instructions=24,relocations=3),
 dict(name='scalar_deleting_dtor',source=F,symbol='??_GCConsoleCommandLine@@UAEPAXI@Z',address=0x9ee390,size=40,instructions=14,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-command-line-ctors.json',[],
  'CConsoleCommandLine header-inline ctors and scalar deleting dtor as retail emitted them out of line (vtable 0x0129C400 RTTI .?AVCConsoleCommandLine@@)')
 print('NATIVE_CONSOLE_COMMAND_LINE_CTORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
