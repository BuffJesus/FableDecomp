"""Boolean text traits linked with native parser providers."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_bool_input
from gate_game_component_abi import Coff
from gate_persist_slong import NAMES
ROOT=Path(__file__).resolve().parents[2]
def main():
 w=Path(tempfile.mkdtemp(prefix='persist-bool-input-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
 retail=oracle_persist_bool_input.collect()
 names=['00/40/CPersistTraitsBool_TransferIn_00403ff0.cpp','00/40/CPersistContext_BoolTransferIn_00403fc0.cpp','00/40/CPersistContext_BoolTransferOut_00403f90.cpp']+[n for n in NAMES[1:] if 'ReadAsInteger' not in n and 'PeekNextItemType' not in n and 'SkipPastNextItemIfIs' not in n];sources=[ROOT/'rebuild/src/compiled'/n for n in names];obj=w/'traits.obj'
 r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(obj);key=next(k for k in c.symbols if k.startswith('?TransferIn@?$CPersistTraits@_N'));sec,off=c.symbols[key];body,rels=c.sections[sec]
 test=ROOT/'rebuild/tests/00/40/CPersistTraitsBool_TransferIn_00403ff0_test.cpp';r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s) for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/persist-bool-input.json').write_text(json.dumps(dict(grade='BEHAVIORAL',compiled_bytes=len(body),retail_bytes=505,linked_cases=12,retail_cases=retail,native_sources=len(sources),mocked_boundaries='string storage and error callbacks'),indent=2)+'\n')
 print('PERSIST_BOOL_INPUT_PASS cases=12 bytes='+str(len(body)))
if __name__=='__main__':main()
