"""Fresh VC71 parity gate: Threaded-file manager deleting destructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CThreadedFileManager@@UAE@XZ':0x98e690,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Destructor_0098e690.cpp'
ROOTS=[
 dict(name='deleting_destructor',source=F,symbol='??_GCThreadedFileManager@@UAEPAXI@Z',address=0x991330,size=30,instructions=11,relocations=2),
]
def main():
 from threaded_manager_destroy_oracle import compare_destroy
 behavior=compare_destroy(deleting=True)
 assert behavior['cases']==5760 and behavior['missing_provider_checks']==6
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-manager-delete.json',[behavior],
  'Threaded-file manager deleting destructor')
 print('NATIVE_THREADED_MANAGER_DELETE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
