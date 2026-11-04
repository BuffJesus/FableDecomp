"""Work-only allocation plus actual resource-registration composition."""
import ast
from pathlib import Path
rootdir=Path(__file__).resolve().parents[3]
fixture=rootdir/'rebuild/tests/integration/NativeGraphicInfoFrameInitialiseComposition_test.py'
text=fixture.read_text();namespace={'__file__':str(fixture)}
exec(compile(text.split('cases=snapshots=0')[0],str(fixture),'exec'),namespace)
globals().update({k:v for k,v in namespace.items() if k not in ('__file__','__builtins__')})
for node in ast.parse(text).body:
 if isinstance(node,(ast.FunctionDef,ast.ClassDef)):exec(compile(ast.Module(body=[node],type_ignores=[]),str(fixture),'exec'),globals())
specs=[
 ('00/9f/CGraphicInfoFrame_CFrameData_Constructor_009fe380.cpp','??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z',0x9fe380,181,10),
 ('00/9f/CResourceBank_AddResource_009fc380.cpp','?AddResource@CResourceBank@@QAEXAAVCResource@@@Z',0x9fc380,52,0),
 ('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,0),
 ('00/9f/CResourceBank_Lifetime_009fc5f0.cpp','??0CResource@@QAE@XZ',0x9fbec0,33,1),
 ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,1)]
mapping.update({symbol:address for path,symbol,address,size,n in specs})
mapping.update({'??_7CFrameData@CGraphicInfoFrame@@6B@':0x129c89c,'??_7CManagedTexture@@6B@':0x129c880,'??_7CResource@@6B@':0x12354a4,'??_7CBaseClass@@6B@':0x1231710,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60})
ctorproofs=[];added_omissions=[]
for path,symbol,address,size,nrels in specs:
 obj=B/('allocation_%x.obj'%address);result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(R/'rebuild/src/compiled'/path)],V.env(),B);assert not result.returncode,result.stdout+result.stderr
 co=Coff(obj);section,offset=co.symbols[symbol];body,rels=co.sections[section];assert offset==0 and len(rels)==nrels
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert name in mapping and kind in (6,20) and body[site:site+4]==word(0),(site,name,kind)
  patched[site:site+4]=word(mapping[name]-(address+site+4) if kind==20 else mapping[name])
 for name,data in [('retail',read_virtual_data(EXE.read_bytes(),address,size)),('compiled',bytes(patched))]:
  u=machines[name]
  try:u.mem_read(address,1)
  except UcError:u.mem_map(address&~4095,4096)
  u.mem_write(address,data)
 p=check(obj,symbol,address,size,nrels,mapping);assert p['near_match'] and p['complete_decode'];assert p['grade']==('NEAR_MATCH' if address==CTOR else 'MATCH'),p;ctorproofs.append(p)
 index=len(ctorproofs)-1;required=Path(__file__).with_name('NativeGraphicInfoFrameInitialiseAllocation_required%d.cpp'%index);ro=B/('allocation_required%d.obj'%index)
 result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(ro),str(required)],V.env(),B);assert not result.returncode,result.stdout+result.stderr
 wrapper=Coff(ro);wrapper_section,wrapper_offset=wrapper.symbols['_RequiredAllocationProvider'];wrapper_body,wrapper_relocs=wrapper.sections[wrapper_section]
 direct=[(wrapper_section,site,kind) for site,(target,kind) in wrapper_relocs.items() if target==symbol]
 assert wrapper_offset==0 and len(direct)==1 and all(kind==20 and wrapper_body[site:site+4]==word(0) for sec,site,kind in direct),(symbol,direct)
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(B/('allocation_required%d.exe'%index)),'kernel32.lib',str(ro)]
 missing=subprocess.run(command,env=V.env(),capture_output=True,text=True);diagnostic=missing.stdout+missing.stderr
 assert missing.returncode and any(('LNK2001' in line or 'LNK2019' in line) and symbol in line for line in diagnostic.splitlines()),diagnostic
 linked=subprocess.run(command+[str(obj)],env=V.env(),capture_output=True,text=True);assert not linked.returncode,linked.stdout+linked.stderr
 added_omissions.append(dict(symbol=symbol,missing_failed=True,provider_link_passed=True,direct_target_relocations=direct))

prior_external=external
def external(m,ctx,name,selfp,args,events,snaps):
 if name=='allocate':
  assert args==(68,);events.append((name,selfp,tuple(args)));snaps.append(m.snap());return ctx['addresses'][2]
 return prior_external(m,ctx,name,selfp,args,events,snaps)
prior_callback=child_callback
def callback(m,ctx,name,selfp,args,events,snaps):
 if name=='allocate':return external(m,ctx,name,selfp,args,events,snaps)
 if name=='construct':
  frame=args[0];p=selfp;listp=ctx['bank']+0x168;head=listp+4
  m.put(p,0x129c89c);m.put(p+4,1)
  for offset in range(8,28,4):m.put(p+offset,0)
  m.put(p+28,0);m.put(p+32,0x129c880);m.put(p+36,0);m.put(p+40,0x129c880)
  for offset in (44,48,52):m.put(p+offset,0)
  m.put(p+64,frame);m.write(p+56,b'\xff');m.write(p+58,b'\x7f');m.put(p+60,m.w(listp+44)-1)
  m.put(p+20,72);m.put(p+8,listp);m.put(p+24,m.w(listp+44))
  tail=m.w(head+12);m.put(p+12,tail);m.put(p+16,head);m.put(tail+16,p);m.put(head+12,p)
  m.put(listp+32,m.w(listp+32)+1);m.put(listp+36,m.w(listp+36)+72)
  return p
 return prior_callback(m,ctx,name,selfp,args,events,snaps)

roottext=(R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialise_test.py').read_text()
exec(ast.get_source_segment(roottext,next(n for n in ast.parse(roottext).body if isinstance(n,ast.FunctionDef) and n.name=='model')),globals())

# Same ABI driver, with genuine ctor execution and one allocator boundary added.
driver=ast.get_source_segment(text,next(n for n in ast.parse(text).body if isinstance(n,ast.FunctionDef) and n.name=='composed_run'))
replacements=[("assert pc not in (ALLOC,CTOR),'excluded allocation partition entered'", "assert pc not in (0x9fcbf0,0xa69e60),'impossible size-query branch entered'"),("boundaries={DELETE:","boundaries={ALLOC:('allocate',1,0),DELETE:"),("selfp=0 if name=='heap_delete'", "selfp=0 if name in ('heap_delete','allocate')"),('external(m,ctx,name,selfp,args,events,snaps)','result=external(m,ctx,name,selfp,args,events,snaps)'),('uc.reg_write(UC_X86_REG_EAX,0);','uc.reg_write(UC_X86_REG_EAX,result);')]
for before,after in replacements:
 assert driver.count(before)==1,(before,driver.count(before));driver=driver.replace(before,after)
provider_addresses=[ENTRY,UNINIT,ASSIGN,RELEASE]+[address for _,_,address,_,_ in specs]
hits={kind:{address:0 for address in provider_addresses} for kind in ('retail','compiled')}
assert driver.count("if pc==STOP:uc.emu_stop();return")==1,'Composition hit-hook insertion changed'
driver=driver.replace("if pc==STOP:uc.emu_stop();return","if pc in hits[kind]:hits[kind][pc]+=1\n  if pc==STOP:uc.emu_stop();return")
exec(driver,globals())
cases=snapshots=0
patterns=(0,1,2,0x7fffffff,0x80000000,0xffffffff)
for mip,age,empty,place,fill in itertools.product(range(256),patterns,range(2),range(2),range(2)):
 initial,ctx=case_initial(mip,1,2,place,fill);m=Memory(initial)
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 bank=A+0x3200;info=A+0x3000;listp=bank+0x168;head=listp+4;tail=A+0x3800
 ctx.update(bank=bank,mutation=0,replacement_data=A+0x3c00)
 m.put(frame+12,info);m.put(info+32,bank)
 m.put(listp+44,age);m.put(listp+32,age);m.put(listp+36,age)
 m.put(head+12,head if empty else tail);m.put(head+16,head if empty else tail);m.put(tail+16,head)
 m.put(incoming,VT);m.put(incoming+4,3)
 expected=model(initial,ctx)
 for kind in ('retail','compiled'):assert composed_run(kind,bytes(initial),ctx)==expected,(mip,age,empty,place,fill,kind)
 cases+=1;snapshots+=len(expected[1])
baseline=dict(cases=cases,snapshots=snapshots)
baseline_hits={kind:dict(value) for kind,value in hits.items()}
extra_cases=extra_snapshots=0
groups={name:dict(cases=0,snapshots=0) for name in ('null_source','notify_unchanged','notify_clears_source','notify_replaces_source','delete_source')}
for mip,group,accounting,place,fill in itertools.product(range(256),groups,((0,0xffffffff),(0xffffffff,0),(2,0x7fffffff)),range(2),range(2)):
 initial,ctx=case_initial(mip,1,2,place,fill);m=Memory(initial)
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 bank=A+0x3200;info=A+0x3000;listp=bank+0x168;head=listp+4;tail=A+0x3800
 mutation={'null_source':0,'notify_unchanged':0,'notify_clears_source':1,'notify_replaces_source':2,'delete_source':0}[group]
 ctx.update(bank=bank,mutation=mutation,replacement_data=src+0x600)
 m.put(frame+12,info);m.put(info+32,bank)
 m.put(listp+44,0xffffffff);m.put(listp+32,accounting[0]);m.put(listp+36,accounting[1])
 m.put(head+12,tail);m.put(head+16,tail);m.put(tail+16,head)
 # Actual assignment AddRef precedes Release: source1 becomes2 (notify), source0 becomes1 (delete).
 m.put(incoming,VT);m.put(incoming+4,0 if group=='delete_source' else 1)
 m.put(ctx['replacement_data'],VT);m.put(ctx['replacement_data']+4,4)
 if group=='null_source':m.put(managed+4,0)
 expected=model(initial,ctx)
 assert any(e[0]=='notify' for e in expected[0])==(group.startswith('notify_')),expected[0]
 assert any(e[0]=='intrusive_delete' for e in expected[0])==(group=='delete_source'),expected[0]
 final=Memory(bytearray(expected[1][-1]));assert final.w(listp+32)==(accounting[0]+1)&0xffffffff and final.w(listp+36)==(accounting[1]+72)&0xffffffff
 assert final.w(managed+4)==0 and final.w(fresh+36)==(0 if group=='null_source' else incoming)
 for kind in ('retail','compiled'):assert composed_run(kind,bytes(initial),ctx)==expected,(mip,group,accounting,place,fill,kind)
 extra_cases+=1;extra_snapshots+=len(expected[1]);groups[group]['cases']+=1;groups[group]['snapshots']+=len(expected[1])
supplement=dict(cases=extra_cases,snapshots=extra_snapshots,groups=groups)
assert all({address for address,count in h.items() if count}==set(provider_addresses)-{UNINIT} for h in hits.values()),hits
inputs=set()
def hash_input(path):
 path=path.resolve()
 if path in inputs:return
 inputs.add(path)
 for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  target=next((p for p in (path.parent/include,R/'rebuild/include'/include) if p.exists()),None)
  assert target is not None,(path,include)
  hash_input(target)
for path in [Path(__file__),fixture,R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialise_test.py']+[p for p,_,_,_ in provider_objects]+[R/'rebuild/src/compiled'/p for p,_,_,_,_ in specs]:hash_input(path)
for index in range(5):hash_input(Path(__file__).with_name('NativeGraphicInfoFrameInitialiseAllocation_required%d.cpp'%index))
for index in range(4):hash_input(fixture.with_name('NativeGraphicInfoFrameInitialiseComposition_required%d.cpp'%index))
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py'):hash_input(R/'tools/decomp_pipeline'/name)
hash_input(R/'tools/artifact_layout.py')
hash_input(R/'tools/decomp_pipeline/gate_native_graphic_info_frame_initialise_allocation.py')
report=dict(cases=cases,snapshots=snapshots,providers=proofs+ctorproofs,input_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)},original_exe_sha256=hashlib.sha256(EXE.read_bytes()).hexdigest(),boundary='Successful allocator boundary returns disjoint fresh 68B arena block. Actual root, CFrameData/resource/base constructors, size accounting, bank registration and managed assignment/release. No behavioral callbacks inside constructor; null Frame and LoadingManagedTexture established by genuine children, impossible size queries abort. Heap allocation, ReleaseTexture and deletions remain external; no resource-bank construction, allocator lifetime, vtable method execution, rendering, source completeness or new recovery count.')
report.update(omission_checks=omissions+added_omissions,installed_providers=9,executed_providers=8,unexecuted_provider='?Uninitialise@CGraphicFrameData@@QAEXXZ')
assert len(report['omission_checks'])==9
assert [p['grade'] for p in report['providers']]==['MATCH','MATCH','MATCH','NEAR_MATCH','NEAR_MATCH','MATCH','MATCH','MATCH','MATCH']
report.update(baseline=baseline,supplement=supplement,total_cases=cases+extra_cases,total_snapshots=snapshots+extra_snapshots,baseline_provider_hits={k:{hex(a):n for a,n in v.items()} for k,v in baseline_hits.items()},provider_hits={k:{hex(a):n for a,n in v.items()} for k,v in hits.items()})
report['boundary']+=' Supplemental all256mips/two placements/fills: incoming managed NULL; incoming rawcount1 becomes2 after actual assignment for notify unchanged/clear/replacement callbacks; incoming rawcount0 becomes1 before deletion. Independent bank count/byte pairs0/FFFFFFFF,FFFFFFFF/0,2/7FFFFFFF with explicit wraparound checks. Measured per-provider original/compiled entries, Uninitialise zero. Typed public required wrappers call actual target via one zero-addend REL32 relocation; link-only executables never run.'
out=R/'rebuild/compile-gate/native-graphic-info-frame-initialise-allocation.json';out.write_text(json.dumps(report,indent=2)+'\n')
print('INITIALISE_ALLOCATION_COMPOSITION_PASS',cases,snapshots,flush=True)
print('INITIALISE_ALLOCATION_SUPPLEMENT_PASS',extra_cases,extra_snapshots,json.dumps(groups),flush=True)
