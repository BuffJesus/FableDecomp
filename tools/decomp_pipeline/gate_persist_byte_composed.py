"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_persist_load_text import NAMES as LOAD
from gate_persist_slong import NAMES as SLONG
from gate_persist_byte_dispatch import NAMES as DISPATCH
import struct
from pe_oracle import EXE,pe_sections,va_to_off



from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=list(dict.fromkeys(DISPATCH+LOAD+SLONG+['00/9b/CPersistContext_TransferObjectSaveText_009bad60.cpp']))
def main():
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/../40/CPersistContext_ByteComposed_00405a60_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?TransferSignedChar@CPersistContext'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_BYTE_COMPOSED_OK cases=8' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/persist-byte-composed.json').write_text(json.dumps(dict(linked_cases=8,native_sources=len(sources),mocked_boundaries='string storage, symbol lookup/sort, CRC, integer formatting and contextual errors; binary dispatch unused'),indent=2)+'\n')
    print('PERSIST_BYTE_COMPOSED_PASS cases=8 sources='+str(len(sources)))
if __name__=='__main__':main()
