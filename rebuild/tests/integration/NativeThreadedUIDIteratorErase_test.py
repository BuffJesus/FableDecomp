"""Fresh UID iterator erase acceptance; compile four genuine sources, never run EXE."""
import argparse,hashlib,importlib.util,json,os,re,struct,subprocess,sys,tempfile,traceback
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=Path(__file__).resolve().parent;R=next(p for p in S.parents if (p/'rebuild').is_dir());sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare
import NativeThreadedUIDIteratorErase_model as model
def inventory(p):
 b=p.read_bytes();machine,n,stamp,sp,ns,opt,flags=struct.unpack_from('<HHIIIHH',b)
 assert machine==0x14c and opt==0
 st=sp+18*ns;syms=[];names={};i=0
 while i<ns:
  raw,val,sec,typ,storage,na=struct.unpack_from('<8sIhHBB',b,sp+18*i)
  if raw[:4]==bytes(4):
   at=st+struct.unpack_from('<I',raw,4)[0];name=b[at:b.index(0,at)].decode('ascii')
  else:name=raw.rstrip(b'\0').decode('ascii')
  aux=[b[sp+18*(i+1+j):sp+18*(i+2+j)].hex() for j in range(na)]
  record=dict(index=i,name=name,value=val,section=sec,type=typ,storage=storage,aux=aux)
  if storage==105:
   assert na==1;default,search=struct.unpack_from('<II',bytes.fromhex(aux[0]));record['weak_default_index']=default;record['weak_search']=search
  syms.append(record);names[i]=name;i+=1+na
 sections=[]
 for k in range(n):
  at=20+40*k;name,virtual,va,size,rp,rel,lines,nr,nl,ch=struct.unpack_from('<8sIIIIIIHHI',b,at)
  assert not rp or rp+size<=len(b)
  payload=b[rp:rp+size] if rp else b'';fix=[]
  for j in range(nr):
   off,idx,kind=struct.unpack_from('<IIH',b,rel+10*j);fix.append(dict(offset=off,index=idx,target=names[idx],kind=kind))
  sections.append(dict(number=k+1,name=name.rstrip(b'\0').decode('ascii'),header_hex=b[at:at+40].hex(),virtual_size=virtual,virtual_address=va,raw_size=size,raw_pointer=rp,relocation_pointer=rel,line_pointer=lines,line_count=nl,flags=ch,payload_hex=payload.hex(),relocations=fix))
 for s in syms:
  if 'weak_default_index' in s:s['weak_default_symbol']=names[s['weak_default_index']]
 return dict(raw_sha256=hashlib.sha256(b).hexdigest(),raw_hex=b.hex(),header_hex=b[:20].hex(),symbols=syms,sections=sections)
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--include',type=Path,default=R/'rebuild/include');parser.add_argument('--provider',type=Path,default=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp');parser.add_argument('--out',type=Path)
 args=parser.parse_args();inc=args.include.resolve();provider=args.provider.resolve();required=S/'NativeThreadedUIDIteratorErase_required.cpp'
 assert not os.environ.get('CL') and not os.environ.get('_CL_'),'Inherited CL option injection'
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);assert not env.get('CL') and not env.get('_CL_')
 D=args.out.resolve() if args.out else Path(tempfile.mkdtemp(prefix='native-uid-iterator-erase-'))
 if args.out: D.mkdir(exist_ok=False,parents=True)
 gate=R/'tools/decomp_pipeline/gate_native_threaded_uid_iterator_erase.py' if S==R/'rebuild/tests/integration' else S/'gate_native_threaded_uid_iterator_erase.py'
 inputs={Path(__file__),S/'NativeThreadedUIDIteratorErase_model.py',gate,model.MODEL,EXE}
 first={};before=None;stage='preflight'
 def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
 def save(name,value):
  p=D/name;assert not p.exists();p.write_text(json.dumps(value,indent=2)+'\n')
 def hashes():return {str(p):sha(p) for p in sorted(inputs)}
 try:
  sources=[provider,required,R/'rebuild/src/compiled/00/42/STLRbTree_RebalanceForErase_0042a34e.cpp',R/'rebuild/src/compiled/00/41/STLRbTree_Extrema_0041c7e0.cpp']
  inputs.update(p for p in inc.rglob('*') if p.is_file())
  inputs.update(p for directory in (V.VC/'include',V.VC/'lib') for p in directory.rglob('*') if p.is_file())
  inputs.update(V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll'))
  inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','verify_and_land.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'))
  inputs.add(R/'tools/artifact_layout.py')
  seen=set();resolution=[]
  def headers(p):
   p=p.resolve()
   if p in seen:return
   seen.add(p);inputs.add(p)
   for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
    q=next((q.resolve() for q in (p.parent/name,inc/name) if q.is_file()),None);assert q and q.is_relative_to(inc),(p,name,q)
    resolution.append(dict(parent=str(p),include=name,resolved=str(q)));headers(q)
  for p in sources:headers(p)
  before=hashes();save('before.json',before);save('quoted_resolution.json',resolution)
  save('environment.json',dict(compiler=str(V.VC/'bin/cl.exe'),linker=str(V.VC/'bin/link.exe'),INCLUDE=env.get('INCLUDE'),LIB=env.get('LIB'),CL=env.get('CL'),_CL_=env.get('_CL_')))
  flags=['/nologo','/c','/O2','/MT','/showIncludes','/I'+str(inc)];objects=[];inventories=[]
  for stem,source in zip(('provider','required','rebalance','extrema'),sources):
   stage='compile '+stem;obj=D/(stem+'.obj');assert not obj.exists()
   command=[*flags,'/Fo'+str(obj),str(source)];result=V.cl(command,env,D)
   save(stem+'.compile.json',dict(command=command,source=str(source),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
   if obj.exists():first[str(obj)]=sha(obj);save(stem+'.raw-first.json',dict(raw_sha256=first[str(obj)],raw_hex=obj.read_bytes().hex()))
   assert result.returncode==0 and obj.exists(),result.stdout+result.stderr
   shown=[]
   for line in (result.stdout+result.stderr).splitlines():
    if 'including file:' in line.lower():
     p=Path(line.split(':',2)[-1].strip()).resolve();assert p in inputs,(stem,p);shown.append(str(p))
   assert shown,'showIncludes produced no header receipt';save(stem+'.showIncludes.json',shown)
   inv=inventory(obj);save(stem+'.inventory.json',inv);first[str(obj)]=sha(obj);objects.append(obj);inventories.append(inv)
  stage='whole body identity'
  definitions=[r for r in inventories[0]['symbols'] if r['section']>0 and r['name'].startswith('?erase@?$_Rb_tree@K') and r['storage']==2];assert len(definitions)==1,definitions
  root=definitions[0];assert root['value']==0;coff=Coff(objects[0]);section,offset=coff.symbols[root['name']];assert offset==0;body,rels=coff.sections[section]
  rebalance='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
  minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';maximum='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
  left='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';right='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
  role_specs=[(root['name'],0,0x990430,50),(rebalance,2,0x42a34e,572),(minimum,3,0x41c7e0,26),(maximum,3,0x41c800,26),(left,2,0x42951b,64),(right,2,0x42955b,64)]
  targets={symbol:address for symbol,index,address,size in role_specs};targets['_free']=0xbfea14;raw=EXE.read_bytes();body_audits=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
  for symbol,index,address,size in role_specs:
   c=Coff(objects[index]);section,offset=c.symbols[symbol];assert offset==0;b,fix=c.sections[section];patched=bytearray(b)
   for site,(name,kind) in fix.items():
    assert kind==20 and name in targets and struct.unpack_from('<I',b,site)[0]==0
    struct.pack_into('<I',patched,site,(targets[name]-address-site-4)&0xffffffff)
   native=read_virtual_data(raw,address,size);score=compare(bytes(patched),native).to_dict();exact=bytes(patched)==native
   decoded=list(md.disasm(b,address));assert sum(i.size for i in decoded)==len(b)
   assert exact and len(b)==size,(symbol,len(b),size)
   assert hashlib.sha256(native).hexdigest()==model.SPECS[address][1]
   body_audits.append(dict(symbol=symbol,address=hex(address),bytes=len(b),original_bytes=size,instructions=len(decoded),grade='MATCH' if exact else 'NEAR_MATCH' if score['near_match'] else 'FAIL',relocated_exact=exact,score=score,relocations={str(site):list(v) for site,v in fix.items()},compiled_hex=patched.hex(),original_hex=native.hex(),disassembly=[dict(address=hex(i.address),bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in decoded]))
  save('body_audits.json',body_audits)
  stage='both concrete caller definitions'
  caller=Coff(objects[1]);assert not any(r['name']==root['name'] and r['section']>0 for r in inventories[1]['symbols'])
  calls=[(section,site,b,rels) for section,(b,rels) in caller.sections.items() for site,(name,kind) in rels.items() if name==root['name'] and kind==20 and struct.unpack_from('<I',b,site)[0]==0];assert len(calls)==2
  md=Cs(CS_ARCH_X86,CS_MODE_32);abi=[]
  expected={'outlined_map':([('mov','edx, dword ptr [esp + 4]'),('push','ecx'),('mov','eax, esp'),('mov','dword ptr [eax], edx')],[('ret','4')]),'generic_call':([('push','ecx'),('mov','ecx, dword ptr [esp + 0xc]'),('mov','eax, esp'),('mov','dword ptr [eax], ecx'),('mov','ecx, dword ptr [esp + 8]')],[('ret','')])}
  for section,site,body,rels in calls:
   owners=[r for r in inventories[1]['symbols'] if r['section']==section and r['storage']==2];assert len(owners)==1 and owners[0]['value']==0
   name=owners[0]['name'];role='outlined_map' if name.startswith('?erase@?$map@') else 'generic_call' if name.startswith('?Call@?$UIDIteratorEraseCaller@') else None;assert role
   assert rels=={site:(root['name'],20)}
   ins=list(md.disasm(body,0));assert sum(i.size for i in ins)==len(body);ci=next(i for i,x in enumerate(ins) if x.mnemonic=='call' and x.address+1==site)
   assert [(x.mnemonic,x.op_str) for x in ins[:ci]]==expected[role][0]
   assert [(x.mnemonic,x.op_str) for x in ins[ci+1:]]==expected[role][1]
   abi.append(dict(role=role,defined_symbol=name,section=section,body_hex=body.hex(),root_relocation=dict(site=site,symbol=root['name'],kind=20,addend=0),this_register='ECX preserved original map' if role=='outlined_map' else 'ECX loaded from static Call map argument',iterator_argument='one caller-copied DWORD value, using actual canonical iterator copy ABI',callee_root_cleanup=4,caller_cleanup=4 if role=='outlined_map' else 0,disassembly=[dict(address=hex(x.address),mnemonic=x.mnemonic,operands=x.op_str) for x in ins]))
  assert {r['role'] for r in abi}=={'outlined_map','generic_call'};save('caller_abi.json',abi)
  directives=[]
  for obj,inv in zip(objects,inventories):
   directives.append(dict(object=str(obj),sections=[dict(number=s['number'],payload_hex=s['payload_hex'],text=bytes.fromhex(s['payload_hex']).decode('ascii'),section_header_hex=s['header_hex']) for s in inv['sections'] if s['name']=='.drectve']))
  assert all(any('LIBCMT' in section['text'] and 'OLDNAMES' in section['text'] for section in row['sections']) for row in directives);save('object_directives.json',directives)
  stage='three source omission/full control links'
  rebalance='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z';minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
  links=[]
  for stem,omit,symbol in (('omit-root',0,root['name']),('omit-rebalance',2,rebalance),('omit-extrema',3,minimum),('control',None,None)):
   cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(D/(stem+'.exe')),*[str(obj) for i,obj in enumerate(objects) if i!=omit]]
   result=subprocess.run(cmd,cwd=D,env=env,capture_output=True,text=True);row=dict(mode=stem,omitted_object=str(objects[omit]) if omit is not None else None,required_symbol=symbol,command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr);save(stem+'.link.json',row);links.append(row)
   if omit is None:assert result.returncode==0,result.stdout+result.stderr
   else:assert result.returncode and any(symbol in line and re.search(r'LNK(?:2001|2019)',line) for line in (result.stdout+result.stderr).splitlines()),row

  for row in body_audits:
   obj=objects[{0x990430:0,0x42a34e:2,0x41c7e0:3,0x41c800:3,0x42951b:2,0x42955b:2}[int(row['address'],16)]]
   row.update(object=str(obj),object_sha256=sha(obj),retail_sha256=hashlib.sha256(bytes.fromhex(row['original_hex'])).hexdigest(),patched_sha256=hashlib.sha256(bytes.fromhex(row['compiled_hex'])).hexdigest())
  save('complete_provider_bodies.json',body_audits)
  original={int(row['address'],16):bytes.fromhex(row['original_hex']) for row in body_audits}
  compiled={int(row['address'],16):bytes.fromhex(row['compiled_hex']) for row in body_audits}
  import pefile
  pe=pefile.PE(data=raw);imports={i.address:(m.dll.decode(),i.name.decode() if i.name else None) for m in pe.DIRECTORY_ENTRY_IMPORT for i in m.imports}
  assert read_virtual_data(raw,model.FREE,6)==bytes.fromhex('ff2554014401') and imports[0x1440154][0].lower()=='msvcr71.dll' and imports[0x1440154][1]=='free'
  stage='independent model per mode'
  domain={mode:model.validate(bodies,D,mode) for mode,bodies in (('original',original),('compiled',compiled))}
  assert domain['original']==domain['compiled']
  after=hashes();save('after.json',after);assert before==after and first=={p:sha(Path(p)) for p in first}
  assert not os.environ.get('CL') and not os.environ.get('_CL_')
  report=dict(status='NATIVE_THREADED_UID_ITERATOR_ERASE_PASS',providers=body_audits,caller_abi=abi,domain=domain,source_omissions=links[:3],link_control=links[3],link_executable_executed=False,input_sha256_before=before,input_sha256_after=after,object_sha256=first,flags=flags,identity_qualification='Concrete tree iterator erase clone required by actual Map forwarding; exclusive historical map/tree folded ownership remains qualified until actual manager caller.',canonical_writes=False)
  save('result.json',report);print(report['status'],flush=True);print('RECEIPT '+str(D/'result.json'),flush=True)
 except BaseException:
  save('failure.json',dict(stage=stage,traceback=traceback.format_exc(),input_before=before,input_after=hashes(),first_object_sha256=first));raise

if __name__=='__main__':main()
