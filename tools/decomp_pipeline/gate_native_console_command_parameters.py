"""Fresh VC71 parity gate: CConsoleCommandParameters one/two/four-type constructors and the vector<EGenericVarType>::push_back they share (lib_console2.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@W4EGenericVarType@@V?$allocator@W4EGenericVarType@@@_STL@@@_STL@@QAEXPAW4EGenericVarType@@ABW43@ABU__false_type@2@I_N@Z':0x9ef900,
}
F=R/'rebuild/src/compiled/00/9e/CConsoleCommandParameters_CConsoleCommandParameters_009ebb80.cpp'
ROOTS=[
 dict(name='ctor1',source=F,symbol='??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z',address=0x9ebb30,size=70,instructions=30,relocations=1),
 dict(name='ctor2',source=F,symbol='??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@0@Z',address=0x9ebb80,size=125,instructions=46,relocations=2),
 dict(name='ctor4',source=F,symbol='??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@000@Z',address=0x9ebcb0,size=221,instructions=86,relocations=4),
 dict(name='push_back',source=F,symbol='?push_back@?$vector@W4EGenericVarType@@V?$allocator@W4EGenericVarType@@@_STL@@@_STL@@QAEXABW4EGenericVarType@@@Z',address=0x9ebd90,size=50,instructions=19,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-command-parameters.json',[],
  'CConsoleCommandParameters one/two/four-type constructors and the vector<EGenericVarType>::push_back they share (lib_console2.cpp)')
 print('NATIVE_CONSOLE_COMMAND_PARAMETERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
