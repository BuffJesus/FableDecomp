"""Actual native bank constructor chain, allocation order and Windows locks."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,shutil,struct,subprocess
import pefile
import check_one as V
import gate_native_bank_initialization as G
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=G.R
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 baseline=R/'rebuild/compile-gate/native-bank-initialization.json';evidence=json.loads(baseline.read_text());assert evidence['status']=='PASS'
 for key in ('input_sha256','object_sha256','python_provenance_sha256'):
  for p,h in evidence[key].items():assert sha(Path(p))==h,p
 accepted=Path(evidence['build'])
 build=R/'rebuild/build/native-bank-initialization-host/00/9d/009d5f80'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 inc=build/'include';shutil.copytree(accepted/'include',inc)
 fixture=R/'rebuild/tests/integration/NativeBankInitializationHost_test.cpp'
 sources=[R/'rebuild/src/compiled'/s[1] for s in G.SPECS]
 inputs={Path(p) for p in evidence['input_sha256']}|{baseline,accepted/'receipt.json',fixture,Path(__file__),V.VC/'bin/link.exe'}
 inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','kernel32.lib','oldnames.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=build/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)+';'+str(build)
 def save(name,data):(build/name).write_text(json.dumps(data,indent=2)+'\n')
 def compile(name,text,allocator=False):
  source=build/(name+'.cpp');source.write_text(text);obj=build/(name+'.obj')
  result=V.cl(['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(inc),'/I'+str(build),*(['/Dmalloc=FixtureBankInitMalloc'] if allocator else []),'/Fo'+str(obj),str(source)],env,build)
  save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
  for line in result.stdout.splitlines():
   if 'including file:' in line.lower():
    p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(build) or p.is_relative_to(V.VC/'include'),p
  return obj
 objects=[];tables=[];records=[]
 addresses=dict(G.ADDRESS);addresses['_FixtureBankInitMalloc']=addresses['_malloc']
 for i,(name,relative,symbol,address,size) in enumerate(G.SPECS):
  obj=compile(name,sources[i].read_text(),True);objects.append(obj);c=Coff(obj);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size;patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20);value=addresses[target]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff)
  assert bytes(patched)==read_virtual_data(EXE.read_bytes(),address,size),symbol
  records.append(dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=s,symbol=n,kind=k) for s,(n,k) in rels.items()]))
  if name in ('bank','async'):
   table='??_7'+('CBankFile' if name=='bank' else 'CBankFileAsync')+'@@6B@';section,offset=c.symbols[table];body,rels=c.sections[section];assert offset==0 and len(body)==80;tables.extend(n for n,k in rels.values())
 cold=set(tables);cold.update(n.replace('??_E','??_G',1) for n in tables if n.startswith('??_E'))
 for obj in objects:
  c=Coff(obj);cold.update(n for body,rels in c.sections.values() for n,k in rels.values() if n.startswith(('??1','?_M_erase@')))
 (build/'cold_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+n+'=_ColdBankVirtual")' for n in sorted(cold))+'\n')
 objects.append(compile('fixture',fixture.read_text()))
 def link(name,providers):
  exe=build/(name+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,providers),'kernel32.lib'],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link('host',objects);assert result.returncode==0,result.stdout+result.stderr
 mapping={}
 for line in exe.with_suffix('.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 fixups=0
 with pefile.PE(str(exe)) as pe:
  for record in records:
   address=mapping[record['symbol']];patched=bytearray.fromhex(record['body'])
   for rel in record['relocations']:
    site,name,kind=rel['site'],rel['symbol'],rel['kind'];value=mapping[name]+struct.unpack_from('<i',patched,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff);fixups+=1
   assert pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,len(patched))==bytes(patched),record['symbol']
 def run(name,program):
  result=subprocess.run([str(program)],cwd=build,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 result=run('positive',exe);assert result.returncode==0 and 'BANK_INITIALIZATION_CHAIN_HOST_PASS cases=24' in result.stdout,result.stdout+result.stderr
 for i in range(7):
  _,result=link('omit-%d'%i,objects[:i]+objects[i+1:]);assert result.returncode,('provider not required',sources[i])
 mutations=[('bank-wrong-alignment',0,'Alignment = 1;','Alignment = 2;'),('bank-wrong-format',0,'BankFormatVersion = 0x73;','BankFormatVersion = 0x72;'),('async-noncached-default',1,'NonCached = false;','NonCached = true;'),('async-disabled-default',1,'DisableAsync = false;','DisableAsync = true;'),('async-stale-restart',1,'LastRestartingData = 0;','LastRestartingData = reinterpret_cast<CBankFileAsyncData*>(1);'),('packed-nonzero-bias',2,'Bias(0)','Bias(1)'),('wide-skip-counter',3,'++g_CWideStringInstanceCount_013BCA20;',';'),('char-skip-counter',4,'++g_CCharStringInstanceCount_013BD800;',';'),('cache-wrong-buffer',5,'(requestedBufferSize + 8) & ~8','requestedBufferSize + 8')]
 controls=[]
 for name,index,old,new in mutations:
  text=sources[index].read_text();assert text.count(old)==1;controls.append((name,index,text.replace(old,new)))
 early=sources[1].read_text().replace('CBankFileAsync::CBankFileAsync()','CBankFileAsync::CBankFileAsync() : LastRestartingData(0)').replace('    LastRestartingData = 0;\n','')
 controls.append(('async-early-restart-store',1,early))
 for name,index,text in controls:
  obj=compile(name,text,True);providers=list(objects);providers[index]=obj;program,result=link(name,providers);assert result.returncode==0,result.stdout+result.stderr
  result=run(name,program);assert result.returncode==80 and 'BANK_INITIALIZATION_HOST_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr)
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),cases=24,actual_windows_locks=12,roots=records,linked_fixups=fixups,omissions=7,controls=[n for n,i,t in controls],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),python_provenance_sha256={str(p):sha(p) for p in (build/'python-provenance').rglob('*.py')},new_roots=0,boundaries=['Actual base/async and all five helper constructors execute with guarded allocator observations. Original root bytes and final linked bodies/targets checked.','Each allocator observation requires async LastRestartingData and flags still contain the fill pattern, including disposal-list allocation. Independent tree/list links, counter wrap, packed defaults and object/allocation guards checked.','Actual Windows critical sections initialized, entered recursively and deleted. Raw fixture cleanup frees owned empty headers after observation; native bank destructor, virtual dispatch, startup parser and factory/read integration remain outside this host proof. Cold virtual/destructor/tree-erase aliases fail if reached.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-initialization-host.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_INITIALIZATION_HOST_PASS cases=24 roots=7 omissions=7 controls=10');print(build/'receipt.json')

if __name__=='__main__':main()
