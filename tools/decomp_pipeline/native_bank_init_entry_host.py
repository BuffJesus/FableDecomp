from pathlib import Path
import sys,json,subprocess,hashlib,re,struct
sys.path.insert(0,'tools/decomp_pipeline')
import check_one as V
from gate_game_component_abi import Coff
import pefile
def build_host(b):
 b=Path(b).resolve();spec=json.loads((b/'inventory.json').read_text());assert not spec['differences'];env=V.env()
 aliases=[];targets={0x42d131:'@FixtureSymbol@12',0x4014a0:'@FixtureCRC@12',0x9b6300:'@FixtureOverflow@28',0x9cbe10:'@FixtureConvert@16',0x99ed10:'@FixtureLower@12',0x99eae0:'@FixtureDestroy@8',0x99ec30:'@FixtureCopy@12',0x9d34e0:'@FixtureInsert@16',0x129aaf4:'_FixtureEmpty'}
 for symbol,address in spec['targets'].items():aliases.append('#pragma comment(linker,"/alternatename:%s=%s")'%(symbol,targets[address]))
 aliases.append('#pragma comment(linker,"/alternatename:@CallInit@20=%s")'%spec['symbol']);(b/'aliases.h').write_text('\n'.join(aliases)+'\n')
 (b/'Fixture.cpp').write_bytes((Path(__file__).resolve().parents[2]/'rebuild/tests/integration/NativeBankInitEntry_test.cpp').read_bytes())
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(b/'Fixture.obj'),str(b/'Fixture.cpp')],env,b);assert not r.returncode,r.stdout+r.stderr
 cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),str(b/'Fixture.obj'),str(b/'root.obj'),'kernel32.lib'];r=subprocess.run(cmd,env=env,cwd=b,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(b/'probe.exe'),str(b/'input.bin'),str(b/'actual.bin')],cwd=b,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
 a=(b/'actual.bin').read_bytes();e=(b/'expected.bin').read_bytes();assert a==e,(next((i for i,(x,y) in enumerate(zip(a,e)) if x!=y),min(len(a),len(e))),len(a),len(e))
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 c=Coff(b/'root.obj');sec,off=c.symbols[spec['symbol']];body,rels=c.sections[sec];fixed=bytearray(body);entry=mapping[spec['symbol']]
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping.get(name,mapping[targets[spec['targets'][name]]])+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 pe=pefile.PE(str(b/'probe.exe'));assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body))
 report=dict(cases=1728,bytes=302,fixups=len(rels),final_body_exact=True,stdout=r.stdout,boundary='Scratch native host with controlled map/CRC/path/string helpers; full4KiB and normalized helper events compared.')
 (b/'host.json').write_text(json.dumps(report,indent=2));return report,mapping,targets
