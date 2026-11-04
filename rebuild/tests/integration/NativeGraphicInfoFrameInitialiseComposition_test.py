"""Work-only existing-owner composition; no allocation/constructor claim."""
import ast,re,tempfile
from pathlib import Path
rootdir=Path(__file__).resolve().parents[3]
basefile=rootdir/'rebuild/tests/integration/NativeGraphicInfoFrameInitialise_test.py'
source=basefile.read_text()
tree=ast.parse(source)
# Execute only the canonical fixture setup and function definitions, not its batch driver.
# Override __file__ for canonical root resolution; isolate build artifacts from its gate.
prefix=source.split('if len(sys.argv)==1:')[0]
builddir=Path(tempfile.mkdtemp(prefix='native-graphic-info-frame-initialise-composition-'))
prefix,build_replacements=re.subn(r'^B=.*?;B\.mkdir\(.*?\)$', lambda _: 'B=Path('+repr(str(builddir))+');B.mkdir(parents=True,exist_ok=True)',prefix,flags=re.M)
assert build_replacements==1,'Canonical fixture build-path declaration changed'
namespace={'__file__':str(basefile)}
exec(compile(prefix,str(basefile),'exec'),namespace)
globals().update({k:v for k,v in namespace.items() if k not in ('__file__','__builtins__')})
# Function globals must use the composition callbacks installed below.
for node in tree.body:
 if isinstance(node,(ast.FunctionDef,ast.ClassDef)):
  exec(compile(ast.Module(body=[node],type_ignores=[]),str(basefile),'exec'),globals())

children=[
 ('rebuild/src/compiled/00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp','?Uninitialise@CGraphicFrameData@@QAEXXZ',UNINIT,69),
 ('rebuild/src/compiled/00/a6/CManagedTexture_Assignment_00a69b20.cpp','??4CManagedTexture@@QAEABV0@ABV0@@Z',ASSIGN,72),
 ('rebuild/src/compiled/00/a6/CManagedTexture_Release_00a69ad0.cpp','?Release@CManagedTexture@@QAE_NXZ',RELEASE,73)]
NOTIFY=0xa69a80
mapping=dict(targets);mapping['?ReleaseTexture@CManagedTextureData@@QAEXXZ']=NOTIFY
proofs=[proof];provider_objects=[(SOURCE,SYMBOL,OBJ,c)]
for index,(path,symbol,address,size) in enumerate(children):
 obj=B/('composition_child%d.obj'%index)
 result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(R/path)],V.env(),B)
 assert not result.returncode,result.stdout+result.stderr
 co=Coff(obj);section,offset=co.symbols[symbol];body,rels=co.sections[section];assert offset==0
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert name in mapping and kind==20 and body[site:site+4]==word(0),(name,kind)
  patched[site:site+4]=word(mapping[name]-(address+site+4))
 for kind,data in [('retail',read_virtual_data(EXE.read_bytes(),address,size)),('compiled',bytes(patched))]:machines[kind].mem_write(address,data)
 childproof=check(obj,symbol,address,size,len(rels),mapping)
 assert childproof['complete_decode'] and childproof['near_match'],childproof
 assert childproof['grade']==('NEAR_MATCH' if address==RELEASE else 'MATCH'),childproof
 proofs.append(childproof);provider_objects.append((R/path,symbol,obj,co))

omissions=[]
for index,(path,symbol,obj,co) in enumerate(provider_objects):
 required=Path(__file__).with_name('NativeGraphicInfoFrameInitialiseComposition_required%d.cpp'%index)
 ro=B/('required%d.obj'%index)
 result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(ro),str(required)],V.env(),B)
 assert not result.returncode,result.stdout+result.stderr
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(B/('required%d.exe'%index)),'kernel32.lib',str(ro)]
 missing=subprocess.run(command,env=V.env(),capture_output=True,text=True)
 diagnostic=missing.stdout+missing.stderr
 assert missing.returncode and any(('LNK2001' in line or 'LNK2019' in line) and symbol in line for line in diagnostic.splitlines()),diagnostic
 linked=subprocess.run(command+[str(obj)],env=V.env(),capture_output=True,text=True)
 assert not linked.returncode,linked.stdout+linked.stderr
 omissions.append(dict(symbol=symbol,missing_failed=True,provider_link_passed=True))

def external(m,ctx,name,selfp,args,events,snaps):
 events.append((name,selfp,tuple(args)));snaps.append(m.snap())
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 if name=='notify':
  # Genuine external ReleaseTexture boundary may mutate the live source handle.
  if ctx['mutation']==1:m.put(managed+4,0)
  elif ctx['mutation']==2:m.put(managed+4,ctx['replacement_data'])
 elif name=='intrusive_delete':
  if ctx['mutation']==3:m.put(managed+4,ctx['replacement_data'])
 elif name=='counted_delete':
  if ctx['mutation']==4:m.put(old+12,replacement)
 elif name=='heap_delete':pass
 else:raise AssertionError(name)
 return 0

def release_managed(m,ctx,handle,events,snaps):
 data=m.w(handle+4)
 if not data:return 1
 released=0
 if m.w(data+4)==2:
  external(m,ctx,'notify',data,(),events,snaps);released=1
 data=m.w(handle+4)
 if data:
  count=(m.w(data+4)-1)&0xffffffff;m.put(data+4,count)
  if not count:external(m,ctx,'intrusive_delete',data,(),events,snaps)
 m.put(handle+4,0)
 return released

def child_callback(m,ctx,name,selfp,args,events,snaps):
 if name=='managed_release':return release_managed(m,ctx,selfp,events,snaps)
 if name=='managed_assign':
  sourcehandle=args[0]
  if m.w(selfp+4)!=m.w(sourcehandle+4):
   release_managed(m,ctx,selfp,events,snaps)
   if m.w(selfp+4)!=m.w(sourcehandle+4):
    previous=m.w(selfp+4)
    if previous:
     count=(m.w(previous+4)-1)&0xffffffff;m.put(previous+4,count)
     if not count:external(m,ctx,'intrusive_delete',previous,(),events,snaps)
    value=m.w(sourcehandle+4);m.put(selfp+4,value)
    if value:m.put(value+4,m.w(value+4)+1)
  return selfp
 if name=='data_uninitialise':
  flags=m.read(selfp+26,1)[0];m.write(selfp+26,bytes([flags&0xef]))
  if flags&4:
   info=m.w(selfp+12)
   if info:
    m.put(info,m.w(info)-1);info=m.w(selfp+12)
    if not m.w(info):
     external(m,ctx,'counted_delete',m.w(info+8),(),events,snaps)
     external(m,ctx,'heap_delete',0,(m.w(selfp+12),),events,snaps)
   m.put(selfp+12,0);m.put(selfp+8,0)
  return 0
 return external(m,ctx,name,selfp,args,events,snaps)
callback=child_callback

def composed_run(kind,initial,ctx):
 u=machines[kind];u.mem_write(A,initial);u.mem_write(S,bytes(65536));sp=S+0x8000
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 u.mem_write(sp,word(STOP)+word(src)+word(managed)+word(ctx['mip']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,frame);u.reg_write(UC_X86_REG_EFLAGS,2)
 for i,r in enumerate(registers):u.reg_write(r,0x11223300+i)
 events=[];snaps=[];m=Memory(u=u)
 boundaries={DELETE:('heap_delete',1,0),IVDELETE:('intrusive_delete',0,0),COUNTDELETE:('counted_delete',0,0),NOTIFY:('notify',0,0)}
 def hook(uc,pc,n,data):
  if pc==STOP:uc.emu_stop();return
  assert pc not in (ALLOC,CTOR),'excluded allocation partition entered'
  if pc not in boundaries:return
  name,nargs,cleanup=boundaries[pc];stack=uc.reg_read(UC_X86_REG_ESP)
  args=tuple(struct.unpack('<'+'I'*nargs,uc.mem_read(stack+4,4*nargs))) if nargs else ()
  selfp=0 if name=='heap_delete' else uc.reg_read(UC_X86_REG_ECX)
  external(m,ctx,name,selfp,args,events,snaps)
  destination=struct.unpack('<I',uc.mem_read(stack,4))[0]
  uc.reg_write(UC_X86_REG_EAX,0);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,destination)
 active[0]=hook
 try:u.emu_start(ENTRY,STOP,count=1500)
 except UcError as error:raise AssertionError((ctx,kind,hex(u.reg_read(UC_X86_REG_EIP)),events)) from error
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
 assert [u.reg_read(r) for r in registers]==[0x11223300+i for i in range(4)]
 snaps.append(m.snap());return events,snaps,None

cases=snapshots=0
for flags,refs,managedrefs,alias,mutation,place in itertools.product((0,4,16,20,255),(1,2,0xffffffff),(0,1,2,3,0xffffffff),range(3),range(5),range(2)):
 initial,ctx=case_initial(flags,5,refs,place,1);m=Memory(initial)
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 replacement_data=src+0x600;destination_data=src+0x500
 ctx.update(mutation=mutation,replacement_data=replacement_data)
 for data in (incoming,destination_data,replacement_data):m.put(data,VT);m.put(data+4,managedrefs)
 m.put(owner+36,destination_data)
 if alias==1:m.put(owner+36,incoming)
 if alias==2:ctx['addresses']=ctx['addresses'][:-1]+(owner+32,)
 expected=model(initial,ctx)
 for kind in ('retail','compiled'):
  actual=composed_run(kind,bytes(initial),ctx)
  assert actual==expected,(flags,refs,managedrefs,alias,mutation,place,kind,actual[0],expected[0])
 cases+=1;snapshots+=len(expected[1])
inputs=set()
def collect(path):
 path=path.resolve()
 if path in inputs:return
 inputs.add(path)
 for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  choices=(path.parent/include,R/'rebuild/include'/include)
  target=next((p for p in choices if p.exists()),None)
  assert target is not None,(path,include)
  collect(target)
for path in [Path(__file__),basefile]+[p for p,_,_,_ in provider_objects]+[Path(__file__).with_name('NativeGraphicInfoFrameInitialiseComposition_required%d.cpp'%i) for i in range(4)]:collect(path)
collect(R/'tools/decomp_pipeline/gate_native_graphic_info_frame_initialise_composition.py')
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py'):collect(R/'tools/decomp_pipeline'/name)
collect(R/'tools/artifact_layout.py')
hashes={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
report=dict(cases=cases,snapshots=snapshots,providers=proofs,omission_checks=omissions,input_sha256=hashes,original_exe_sha256=hashlib.sha256(EXE.read_bytes()).hexdigest(),boundary='Existing nonnull owner only. Actual Uninitialise/managed assignment/release. ReleaseTexture, virtual deletion, counted deletion and heap deletion callbacks; allocation/CFrameDataCtor excluded. Full16KiB arenas and callback/final snapshots; no source completeness or new recovery count.')
out=R/'rebuild/compile-gate/native-graphic-info-frame-initialise-composition.json';out.write_text(json.dumps(report,indent=2)+'\n')
print('INITIALISE_COMPOSITION_PASS',cases,snapshots,flush=True)
