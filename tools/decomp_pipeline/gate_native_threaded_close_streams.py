"""Fresh near-match and behavioral gate for native file-stream cleanup."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,shutil,subprocess,struct
R=Path.cwd();sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
import gate_native_bank_shared_worker as G
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
from near_match import compare,retail_relocations
root=R/'rebuild/tests/integration'
MODEL=root/'NativeThreadedCloseStreams_model.py';MULTIPLE=root/'NativeThreadedCloseStreams_multiple.py'
out=R/'rebuild/build/native-threaded-close-streams/00/98/0098e7d0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');out.mkdir(parents=True)
inc=out/'include';inc.mkdir();src=out/'sources';src.mkdir()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
providers=[G.SOURCES[i] for i in (19,22,23,27,29,30,31,32,33,34,35,38,47)]
providers.append(R/'rebuild/src/compiled/00/46/UlongVector_Overflow_00464aab.cpp')
providers.append(R/'rebuild/src/compiled/00/98/CThreadedFileManager_CloseAllStreamsForFile_0098e7d0.cpp')
fixtures=[root/('NativeThreadedCloseStreams'+n+'_test.cpp') for n in ('Pending','Active','Reentrant')]
inputs=set()
def closure(p):
 if p in inputs:return
 inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(p,name);closure(q)
for p in providers+fixtures:closure(p)
inputs.update([Path(__file__),MODEL,MULTIPLE,EXE])
inputs.update((V.VC/'include').rglob('*.h'))
inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
inputs.update(V.VC/'lib'/n for n in ('kernel32.lib','libcmt.lib','oldnames.lib'))
inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','native_exact_roots.py','near_match.py','gate_native_bank_shared_worker.py'))
pins={str(p):sha(p) for p in sorted(inputs)}
for p in inputs:
 if p.is_relative_to(R/'rebuild/include'):
  q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
for p in (Path(__file__),MODEL,MULTIPLE):shutil.copy2(p,out/p.name)
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
def save(name,data):
 p=out/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
def compile(name,text):
 p=src/(name+'.cpp');p.write_text(text);obj=out/(name+'.obj')
 r=V.cl(['/nologo','/c','/O2','/MT','/showIncludes','/I'+str(inc),'/Fo'+str(obj),str(p)],env,out)
 save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
 for line in r.stdout.splitlines():
  if 'including file:' in line.lower():
   q=Path(line.split(':',2)[-1].strip()).resolve();assert q.is_relative_to(out) or q.is_relative_to(V.VC/'include'),q
 return obj
objects=[compile('provider%d'%i,p.read_text()) for i,p in enumerate(providers[:-1])]
candidate=compile('candidate',providers[-1].read_text());objects.append(candidate)
# Machine models consume this exact frozen name.
(out/'candidate.cpp').write_bytes((src/'candidate.cpp').read_bytes())
coffs=[Coff(p) for p in objects]
helper_addresses={}
for c in coffs:
 for name,(section,offset) in c.symbols.items():
  if section not in c.sections:continue
  for marker,address in (('?_Rebalance_for_erase@',0x42a34e),('?_S_minimum@',0x41c7e0),('?_S_maximum@',0x41c800),('?_Rotate_left@',0x42951b),('?_Rotate_right@',0x42955b),('?_M_insert_overflow@?$vector@K',0x464ace)):
   if name.startswith(marker):helper_addresses[name]=address
addresses=dict(helper_addresses);addresses.update({'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6,'??3@YAXPAX@Z':0xbfe9bc})
reverse={v:k for k,v in import_names(EXE.read_bytes()).items()}
symbol='?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z'
c=coffs[-1];section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0 and len(body)==840
for site,(name,kind) in rels.items():
 if name.startswith('__imp__'):addresses[name]=reverse[name[7:].split('@')[0]]
 elif '_M_find' in name:addresses[name]=0x98fe70
 elif name.startswith('?find@'):addresses[name]=0x9902d0
 elif '_M_lower_bound' in name:addresses[name]=0x9901d0
 elif '_M_upper_bound' in name:addresses[name]=0x98fca0
 elif '?erase@' in name:addresses[name]=0x9905d0
def patched(body,rels,entry,targets):
 result=bytearray(body)
 for site,(name,kind) in rels.items():
  assert kind in (6,20)
  value=targets[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0);struct.pack_into('<I',result,site,value&0xffffffff)
 return bytes(result)
original=read_virtual_data(EXE.read_bytes(),0x98e7d0,840)
rebuilt=patched(body,rels,0x98e7d0,addresses)
differences=[i for i,(a,b) in enumerate(zip(rebuilt,original)) if a!=b]
assert differences==[493,494,495,496,497,498,499,500],differences
score=compare(body,original,tuple(rels),retail_relocations('0098e7d0',840)).to_dict();assert score['near_match']
helpers=[]
for name,address in helper_addresses.items():
 c=next(c for c in coffs if name in c.symbols and c.symbols[name][0] in c.sections)
 section,offset=c.symbols[name];code,fixups=c.sections[section];assert offset==0
 final=patched(code,fixups,address,addresses);assert final==read_virtual_data(EXE.read_bytes(),address,len(code)),name
 helpers.append(dict(symbol=name,address=address,bytes=len(code),patched=final.hex(),body=code.hex(),relocations=[dict(site=s,symbol=n,kind=k) for s,(n,k) in fixups.items()]))
save('helper-code.json',dict(vector=next(r for r in helpers if r['address']==0x464ace),tree=[r for r in helpers if r['address']!=0x464ace]))
def python_test(script,name):
 r=subprocess.run([sys.executable,str(out/script),str(out)],cwd=R,capture_output=True,text=True,timeout=90)
 save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
python_test(MODEL.name,'single')
python_test(MULTIPLE.name,'multiple')
print('Fresh root and six exact helpers: 720 single and 280 native-container cases passed',flush=True)
def link(name,fixture,providers):
 exe=out/(name+'.exe')
 r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),str(fixture),*map(str,providers),'kernel32.lib'],env=env,cwd=out,capture_output=True,text=True)
 save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
def run(exe,name):
 r=subprocess.run([str(exe)],cwd=out,capture_output=True,text=True,timeout=15)
 save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return r
fixture_objects=[];linked_audits=[]
for fixture,count in zip(fixtures,(24,48,32)):
 name=fixture.stem;obj=compile(name,fixture.read_text());fixture_objects.append(obj)
 exe,r=link(name,obj,objects);assert r.returncode==0,r.stdout+r.stderr
 mapping={}
 for line in exe.with_suffix('.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 assert patched(body,rels,mapping[symbol],mapping)==read_virtual_data(exe.read_bytes(),mapping[symbol],len(body))
 for helper in helpers:
  fixups={r['site']:(r['symbol'],r['kind']) for r in helper['relocations']};code=bytes.fromhex(helper['body'])
  assert patched(code,fixups,mapping[helper['symbol']],mapping)==read_virtual_data(exe.read_bytes(),mapping[helper['symbol']],len(code)),helper['symbol']
 r=run(exe,name);assert r.returncode==0 and 'cases='+str(count) in r.stdout,r.stdout+r.stderr
 linked_audits.append(dict(fixture=name,cases=count,root_bytes=len(body),root_fixups=len(rels),exact_helpers=len(helpers),exe_sha256=sha(exe)))
controls=[('wrong-file','first->second->PFile==file','first->second->PFile!=file',0),('no-block','++BlockPendingObjectProcessingRefCount;',';',0),('success-cancel','OnFileReadCompletion(false)','OnFileReadCompletion(true)',0),('truncated-ids','unsigned long count=ids.size();','unsigned long count=ids.size()?1:0;',2),('no-wakeup','            SetEvent(Event);','            ;',1)]
for name,old,new,which in controls:
 text=providers[-1].read_text();assert text.count(old)==1,(name,text.count(old))
 obj=compile(name,text.replace(old,new));exe,r=link(name,fixture_objects[which],objects[:-1]+[obj]);assert r.returncode==0,r.stdout+r.stderr
 r=run(exe,name);assert r.returncode==80 and 'PENDING_CLOSE_FAILURE' in r.stderr,(name,r.returncode,r.stderr)
missing,r=link('missing-root',fixture_objects[0],objects[:-1]);assert r.returncode!=0 and symbol in r.stdout+r.stderr
assert all(sha(Path(p))==h for p,h in pins.items())
save('receipt.json',dict(status='PASS',grade='NEAR_MATCH',root_bytes=840,original_byte_match=False,original_differing_offsets=differences,similarity=score,exact_helpers=[dict(symbol=r['symbol'],address=r['address'],bytes=r['bytes']) for r in helpers],single_cases=720,multiple_cases=280,host=linked_audits,controls=[c[0] for c in controls],missing_root_rejected=True,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects+fixture_objects},new_roots=0,boundaries=['Fresh ordinary C++ root differs from original only at eight cursor-comparison scheduling bytes; explicit NEAR_MATCH acceptance, not an exact-byte claim.','Actual exact vector/tree helpers in multirequest emulation and host fixtures; heap/Win32 boundaries controlled in emulation.','Real critical section, pending cancellation, controlled active worker and reentrant callback host cases. This gate does not execute production read worker; prior bank/read composition is separate evidence.']))
(R/'rebuild/compile-gate/native-threaded-close-streams.json').write_bytes((out/'receipt.json').read_bytes())
print('NATIVE_THREADED_CLOSE_STREAMS_PASS near_match=1 single=720 multiple=280 pending=24 active=48 reentrant=32 controls=5',flush=True);print(out,flush=True)
