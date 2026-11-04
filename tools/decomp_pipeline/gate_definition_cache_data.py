"""Typed global initializers compared directly with retail image bytes."""
import json,tempfile,struct
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
 w=Path(tempfile.mkdtemp(prefix='definition-cache-data-'));e=check_one.env();raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
 specs=[('01/38/DefinitionCacheData_0138e190.cpp',[('g_DefCacheManager_0138E190',0x138e190,4),('g_DefCacheOffset_0138E194',0x138e194,4),('g_DefCacheSize_0138E198',0x138e198,4),('g_DefCacheData_0138E19C',0x138e19c,0x8000)]),('01/37/IMEAlphaDefault_013750b0.cpp',[('g_IMEAlphaDefault_013750B0',0x13750b0,1)])]
 for index,(source,items) in enumerate(specs):
  obj=w/(str(index)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(obj)
  for name,address,size in items:
   key=next(k for k in c.symbols if k.startswith('?'+name+'@@'));sec,offset=c.symbols[key];body,relocs=c.sections[sec];assert not relocs
   # COFF BSS stores its zero-filled extent without initialized payload.
   extent,raw_pointer=struct.unpack_from("<II",c.data,20+40*(sec-1)+16);assert offset+size<=extent
   value=body[offset:offset+size] if raw_pointer else bytes(size);assert len(value)==size
   retail_offset=va_to_off(sections,address);assert value==raw[retail_offset:retail_offset+size],name;rows.append(dict(name=name,address=hex(address),bytes=size))
 (ROOT/'rebuild/compile-gate/definition-cache-data.json').write_text(json.dumps(dict(grade='DATA_MATCH',providers=rows),indent=2)+'\n');print('DEFINITION_CACHE_DATA_PASS bytes=32781')
if __name__=='__main__':main()
