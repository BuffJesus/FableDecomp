"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import struct
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_definition_destructor




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/CDefinitionManager_Destructor_009b02e0.cpp','00/9b/STL_RecompiledDefTreeErase_009b40f0.cpp']
def main():
    retail=oracle_definition_destructor.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/CDefinitionManager_Destructor_009b02e0_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('??1CDefinitionManager@@'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_DTOR_OK cases=4' in r.stdout,r.stdout
    erase=w/'erase.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(erase),str(sources[1])],e,w);assert r.returncode==0,r.stdout+r.stderr;coff=Coff(erase);key=next(k for k in coff.symbols if k.startswith('?_M_erase'));section,offset=coff.symbols[key];body,relocations=coff.sections[section];patched=bytearray(body)
    for position,(symbol,kind) in relocations.items():
        target=0x9b40f0 if symbol==key else 0xbfea14 if symbol=='_free' else None;assert target is not None and kind==20;struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-0x9b40f0-position-4)&0xffffffff)
    raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0x9b40f0);assert len(body)==75 and bytes(patched)==raw[offset:offset+75]
    (ROOT/'rebuild/compile-gate/definition-destructor.json').write_text(json.dumps(dict(address='009b02e0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=387,linked_cases=4,retail_cases=retail,erase_grade='RELOCATION_MATCH',erase_bytes=75,mocked_boundaries='ClearInstantiatedDefs, string destruction and resource-list destruction; tree erase and counted-pointer release are native'),indent=2)+'\n')
    print('DEFINITION_DTOR_PASS cases=4 bytes='+str(len(b)))
if __name__=='__main__':main()
