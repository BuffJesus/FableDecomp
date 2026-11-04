"""Native filename multimap insertion with exact code, balancing and duplicates."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankFilenameTree_model as M
SOURCES=[R/'rebuild/src/compiled/00/9d/BankFilenameTree_InsertEqual_009d34e0.cpp',R/'rebuild/src/compiled/00/42/BankFilenameTree_InsertNode_0042bcb5.cpp',R/'rebuild/src/compiled/00/42/CBasicStringchar_operator_00429950.cpp',R/'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp']
FIXTURE=R/'rebuild/tests/integration/NativeBankFilenameTree_test.cpp'
ENTRIES=[(0,'?insert_equal',0x9d34e0,93,2),(1,'?_M_insert',0x42bcb5,149,4),(1,'?_M_create_node',0x42b587,33,2),(1,'??0?$pair',0x4297db,27,1),(1,'??R?$less',0x42aab3,41,1),(2,'??M?$CBasicString',0x429950,88,0),(3,'?_Rebalance@',0x42971d,190,4),(3,'?_Rotate_left',0x42951b,64,0),(3,'?_Rotate_right',0x42955b,64,0)]
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
 b=R/'rebuild/build/native-bank-filename-tree/00/9d/009d34e0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];coffs=[]
 for name in ('root0','root1','root2','root3','Fixture'):
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT',*(['/Dmalloc=MultimapMalloc'] if name=='root1' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);coffs.append(Coff(obj))
 specs=[]
 for n,prefix,a,l,f in ENTRIES:
  names=[s for s in coffs[n].symbols if s.startswith(prefix)];assert len(names)==1;specs.append((n,names[0],a,l,f))
 targets={s:a for _,s,a,_,_ in specs};targets.update(_MultimapMalloc=0xbfea0e);targets['??0CCharString@@QAE@ABV0@@Z']=0x99ec30
 proof=[check(objects[n],s,a,l,f,targets) for n,s,a,l,f in specs];candidate={};original={};image=EXE.read_bytes()
 for n,s,a,l,f in specs:
  section,offset=coffs[n].symbols[s];body,rels=coffs[n].sections[section];fixed=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-a-site-4)&0xffffffff)
  candidate[a]=bytes(fixed);original[a]=read_virtual_data(image,a,l);assert candidate[a]==original[a]
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*[str(objects[n]) for n in (2,1,3,0,4)]];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for n,s,a,l,f in specs:
  section,offset=coffs[n].symbols[s];body,rels=coffs[n].sections[section];assert offset==0;fixed=bytearray(body);entry=mapping[s]
  for site,(name,kind) in rels.items():
   assert kind==20;struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),s;file_offsets[a]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=1600' in result.stdout
 cases=M.compare(candidate,original);assert cases==1600
 faults=[('duplicate-search-branch',0x9d34e0,0x31,b'\x75'),('pair-value-copy',0x4297db,0x11,b'\x90'*3),('allocation-size',0x42b587,2,b'\x20'),('node-count',0x42bcb5,0x84,b'\x90'*3),('leftmost-link',0x42bcb5,0x6a,b'\x90'*3),('rebalance',0x42bcb5,0x7f,b'\x90'*5)]
 rebalance=next(s for n,s,a,l,f in specs if a==0x42971d);section,_=coffs[3].symbols[rebalance];_,rels=coffs[3].sections[section]
 for prefix in ('?_Rotate_left','?_Rotate_right'):
  site=next(site for site,(name,kind) in rels.items() if name.startswith(prefix));faults.append((prefix,0x42971d,site-1,b'\x90'*5))
 controls=[]
 for i,(name,entry,offset,replacement) in enumerate(faults):
  changed=dict(original);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in itertools.product((1,4,9,17),(0,1,2,3),(0,3),(0,1)):
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'MULTIMAP_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in range(4):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];cmd.append('/map:'+str(b/('missing-%d.map'%n)));result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  if n!=2:assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
  else:
   assert not result.returncode;comparator=next(s for _,s,a,_,_ in specs if a==0x429950);line=next(line for line in (b/'missing-2.map').read_text().splitlines() if comparator in line);address=int(line.split()[2],16);missing_pe=pefile.PE(str(b/'missing-2.exe'));assert missing_pe.get_data(address-missing_pe.OPTIONAL_HEADER.ImageBase,88)!=original[0x429950]
   result=subprocess.run([str(b/'missing-2.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('missing-2.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=1600' in result.stdout
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),model_cases=cases,host_cases=1600,proof=proof,controls=controls,required_provider_omissions=3,redundant_comparator_omission=dict(runtime_neutral=True,exact_body_audit_rejected=True),input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Nine exact original/final-linked bodies. Duplicate keys preserve insertion order and values; signed string ordering, parent links, red-black invariants, header extrema and count verified. Original/rebuilt full128KiB arenas compared. Native guarded CRT allocations released once. CCharString copy/destruction controlled; no allocation failure/unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-filename-tree.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_FILENAME_TREE_PASS model=1600 host=1600 controls=16 bodies=9')
if __name__=='__main__':main()
