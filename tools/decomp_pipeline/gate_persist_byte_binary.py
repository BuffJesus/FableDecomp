"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_byte_binary



from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/40/CPersistTraitsByte_TransferBinaryIn_00405940.cpp']
def main():
    retail=oracle_persist_byte_binary.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/../40/CPersistTraitsByte_TransferBinaryIn_00405940_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?TransferBinaryIn@?$CPersistTraits@E'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_BYTE_BINARY_OK cases=12' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/persist-byte-binary.json').write_text(json.dumps(dict(address='00405940',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=142,linked_cases=12,retail_cases=retail,mocked_boundaries='source-window overflow; rejected reads outside verification scope'),indent=2)+'\n')
    print('PERSIST_BYTE_BINARY_PASS cases=12 bytes='+str(len(b)))
if __name__=='__main__':main()
