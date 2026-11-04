"""Native bank filename path conversion with wide/narrow ownership."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativeBankInitCRC_model as M
SOURCES=[R/"rebuild/src/compiled"/n for n in ['00/9c/CBankFile_InitEntry_009ce050.cpp', '00/40/Global_CCRC_Calc_004014a0.cpp', '01/29/FableCRCTable_0129a168.cpp']]
FIXTURE=R/"rebuild/tests/integration/NativeBankInitCRC_test.cpp"
SPECS=[{'bytes': 302,
  'differences': [],
  'entry': '0x9ce050',
  'fixups': 13,
  'owner': 'root0.obj',
  'symbol': '?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z',
  'targets': {'??0CCharString@@QAE@ABV0@@Z': '0x99ec30',
              '??1CCharString@@QAE@XZ': '0x99eae0',
              '??A?$map@VCCharString@@KU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEAAKABVCCharString@@@Z': '0x42d131',
              '?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z': '0x9cbe10',
              '?FableCRC_Calc_004014A0@@YIIIPBXI@Z': '0x4014a0',
              '?GetAsLowerCase@CCharString@@QBE?AV1@XZ': '0x99ed10',
              '?_M_insert_overflow@?$vector@U?$pair@KK@_STL@@V?$allocator@U?$pair@KK@_STL@@@2@@_STL@@QAEXPAU?$pair@KK@2@ABU32@ABU__false_type@2@I_N@Z': '0x9b6300',
              '?g_FableEmptyNarrowString_0129AAF4@@3PADA': '0x129aaf4',
              '?insert_equal@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x9d34e0'}},
 {'bytes': 259,
  'differences': [],
  'entry': '0x9b6300',
  'fixups': 2,
  'owner': 'root0.obj',
  'symbol': '?_M_insert_overflow@?$vector@U?$pair@KK@_STL@@V?$allocator@U?$pair@KK@_STL@@@2@@_STL@@QAEXPAU?$pair@KK@2@ABU32@ABU__false_type@2@I_N@Z',
  'targets': {'_CRCFree': '0xbfea14', '_CRCMalloc': '0xbfea0e'}},
 {'bytes': 56,
  'differences': [],
  'entry': '0x4014a0',
  'fixups': 1,
  'owner': 'root1.obj',
  'symbol': '?FableCRC_Calc_004014A0@@YIIIPBXI@Z',
  'targets': {'?g_FableCRCTable_0129A168@@3PAIA': '0x129a168'}},
 {'bytes': 1024,
  'differences': [],
  'entry': '0x129a168',
  'fixups': 0,
  'owner': 'root2.obj',
  'symbol': '?g_FableCRCTable_0129A168@@3PAIA',
  'targets': {}}]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 
def main():
 inputs={Path(__file__),Path(M.__file__),EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in SOURCES+[FIXTURE]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-init-crc/00/9c/009ce050'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];coffs=[]
 for name in ['root%d'%i for i in range(len(SOURCES))]+['Fixture']:
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Gy',*(['/Dmalloc=CRCMalloc','/Dfree=CRCFree'] if name=='root0' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);coffs.append(Coff(obj))

 candidate={};original={};image=EXE.read_bytes();proof=[]
 for row in SPECS:
  n=int(row['owner'][4:-4]);c=coffs[n];section,offset=c.symbols[row['symbol']];body,rels=c.sections[section];assert offset==0 and len(body)==row['bytes'] and len(rels)==row['fixups']
  entry=int(row['entry'],16);fixed=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);target=int(row['targets'][{'_PathMalloc':'_malloc','_PathFree':'_free'}.get(name,name)],16);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  candidate[entry]=bytes(fixed);original[entry]=read_virtual_data(image,entry,len(body));assert candidate[entry]==original[entry],row['symbol'];proof.append(row)
 command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for row in SPECS:
  n=int(row['owner'][4:-4]);symbol=row['symbol'];section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];fixed=bytearray(body);entry=mapping[symbol]
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),symbol;file_offsets[int(row['entry'],16)]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=1056' in result.stdout
 cases=M.compare(candidate,original);assert cases==1056


 faults=[('crc-flag',0x9ce050,0x29,b'\x02'),('payload-store',0x9ce050,0x68,b'\x90'*3),('dirty-flag',0x9ce050,0x8a,b'\x00'),('growth-old-payload',0x9b6300,0x5b,b'\x90'*3),('growth-new-payload',0x9b6300,0x7c,b'\x90'*3),('growth-old-free',0x9b6300,0xe4,b'\x90'*5),('crc-shift',0x4014a0,0x2a,b'\x07'),('crc-table',0x129a168,4,b'\x97')]
 controls=[]
 for i,(name,entry,offset,replacement) in enumerate(faults):
  changed=dict(original);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in itertools.product((1,2,9,17),(0,1,7,33),(0,1,2,3),(0,1)):
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'INIT_CRC_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in (0,1,2):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),model_cases=cases,host_cases=1056,proof=proof,controls=controls,required_provider_omissions=3,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Native InitEntry CRC branch: three exact original/final code bodies (entry302B, CRC56B, pair-vector growth259B) plus full1024-byte original/final CRC table. 1056 differential and native guarded scenarios cover empty/allocated/null counted strings, embedded zero/high bytes, existing capacity and repeated growth. Independent bitwise CRC oracle, ordered key/payload preservation, Dirty flag, source preservation and exact capacity growth checked. Full128KiB original/rebuilt arena digests and allocation events agree; all guarded allocations released. Filename/symbol branches fail fast. Final vector storage is manually released; no arbitrary insertion/growth count, allocation-failure/unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-init-crc.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_INIT_CRC_PASS model=1056 host=1056 controls=16 bodies=3 table=1024')
if __name__=='__main__':main()
