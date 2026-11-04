"""Persistent primitive creation/update with independent lifetime/routing model."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-persistent-2d-lists-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_SetPersistentPrimitive_00b324a0.cpp','01/43/NEngineInternal_GPPrimitiveTypeInfo_01436e84.cpp','01/43/NEngineInternal_GPPrimitiveRenderer2D_01436e7c.cpp','00/b4/CEnginePrimitiveRenderer2D_AddPrimitive_00b4a670.cpp','00/b8/CEngineInternalPrimitiveBase_Lists_00b8fdf0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativePersistent2DLists_test.cpp'
symbol='?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z'
typeinfo='?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A';twoD='?GPPrimitiveRenderer2D@NEngineInternal@@3PAVCEnginePrimitiveRenderer2D@@A'
update='?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z';add2D='?AddPrimitive@CEnginePrimitiveRenderer2D@@QAEXAAVCEngineInternalPrimitiveBase@@@Z';add='?AddPrimitive@CEnginePrimitiveManager@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z'
targets={typeinfo:0x1436e84,twoD:0x1436e7c,update:0xb91840,add2D:0xb4a670,add:0xbacdd0}
aliases={'@Persistent@24':symbol,'_TypeGlobal':typeinfo,'_TwoDGlobal':twoD,update:'@SceneUpdate@8',add:'@ManagerAdd@16'}
(w/'persistent_2d_lists_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
listadd='?AddToList@CEngineInternalPrimitiveBase@@QAEXPAPAV1@@Z';targets[listadd]=0xb8fdf0
raw=EXE.read_bytes();body=bytearray(522);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(0,symbol,0xb324a0,354,5,0),(3,add2D,0xb4a670,38,2,368),(4,listadd,0xb8fdf0,106,0,416)]
for n,sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x1436e84,0x1436e7c):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==7
(w/'persistent_2d_lists_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,address,length,count,targets) for n,sym,address,length,count,offset in specs]
for n,(sym,address) in enumerate(((typeinfo,0x1436e84),(twoD,0x1436e7c)),1):assert coff_data(Coff(objects[n]),sym,4)==read_virtual_data(raw,address,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for typ,old,present,flags,mode,np in itertools.product(range(47),range(3),range(2),(0,0x20,0x40,0x60,1,4,0xffffffff,0x80000000),range(6),range(2)):
  refs=(0,1,2,0xffffffff)[(typ+flags+mode)%4];level=(0,0x7f,0x80,0xff)[(typ+old+mode)%4];f.write(struct.pack('<8I',typ,old,present,flags,mode,np,refs,level));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4104==0;snapshots=size//4104
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Independent creation/update/lifetime state model compares every callback entry and final full4KiB plus actual TypeInfo/2D globals.47types,null/same/different existing primitive,manager present/null,eightflagwords,bothnonpersistent values,sixcallbackmodes,rotating reference counts/signedgridlevels. Factory returns nonnull;callbacks clear/replace handle,change refcounts,type/global/flags/gridstate. Checks cached manager/type/enable,reference adoption,virtualslot1factory/5update/3notify/1delete,sceneupdate vsmanager/2D routing. Actual2D routing and grouped-list insertion now composed;independent graph model includes0-2existingnodes per selected/unselectedhead andmatching/newmasks. Factory,primitive virtuals,sceneupdate and3Dmanager routing remaincallbacks.')
(ROOT/'rebuild/compile-gate/native-persistent-2d-lists.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PERSISTENT_2D_LISTS_GATE_PASS',snapshots,'snapshots')
