"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_compress_block




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_CompressDefinitionBlock_009ae390.cpp','00/99/CMemoryDataOutputStream_Clear_00996b60.cpp']
def main():
    retail=oracle_definition_compress_block.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_CompressDefinitionBlock_009ae390_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?CompressDefinitionBlock@CDefinitionManager'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_COMPRESS_BLOCK_OK cases=4' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-compress-block.json').write_text(json.dumps(dict(address='009ae390',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=600,linked_cases=4,retail_cases=retail,mocked_boundaries='memory output stream storage and compressor runtime; vector growth and clearing are native'),indent=2)+'\n')
    print('DEFINITION_COMPRESS_BLOCK_PASS cases=4 bytes='+str(len(b)))
if __name__=='__main__':main()
