"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile,struct
from pathlib import Path
import check_one
import oracle_definition_load
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_LoadDef_009ad6e0.cpp','00/99/CMemoryDataInputStream_DestructorTyped_00993b10.cpp','00/9b/CPersistContext_CPersistContext_009baa30.cpp','00/9f/CResource_SetResourceSize_009fbf20.cpp']
def main():
    retail_cases=oracle_definition_load.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_LoadDef_009ad6e0_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?LoadDef'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    dtor_obj=w/'stream-dtor.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(dtor_obj),str(sources[1])],e,w);assert r.returncode==0,r.stdout+r.stderr
    d=Coff(dtor_obj);sec,off=d.symbols['??1CMemoryDataInputStream@@UAE@XZ'];db,drels=d.sections[sec];patched=bytearray(db);assert len(db)==11 and off==0
    for pos,(symbol,kind) in drels.items():
        dest={'??_7CDataStream@@6B@':0x129a69c,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300}[symbol];struct.pack_into('<I',patched,pos,(dest+struct.unpack_from('<i',db,pos)[0]-0x993b10-pos-4 if kind==20 else dest)&0xffffffff)
    raw=EXE.read_bytes();off=va_to_off(pe_sections(raw),0x993b10);assert bytes(patched)==raw[off:off+11]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_LOAD_OK cases=7' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-load.json').write_text(json.dumps(dict(address='009ad6e0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=526,linked_cases=7,retail_oracle_cases=retail_cases,native_sources=len(sources),stream_destructor_parity="RELOCATION_MATCH:11",mocked_boundaries='string storage, decompression, class lookup, stream construction, allocation/virtual transfer, resource list and progress display'),indent=2)+'\n');print('DEFINITION_LOAD_PASS cases=7 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
