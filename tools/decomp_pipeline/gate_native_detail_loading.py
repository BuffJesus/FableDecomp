"""Compare actual scenery-type palette loading with the original implementation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-loading-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_PaletteLoad_00bdedd0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailLoading_test.cpp'
registry_source=ROOT/'rebuild/src/compiled/00/b5/CEngineLocalDetailGenerator_RegisterObjectCollectionType_00b59c70.cpp'
copy_source=ROOT/'rebuild/src/compiled/00/be/CLocalDetailObjectCollectionType_CopyCompare_00be2680.cpp'
extra=[]
for n,p in enumerate((registry_source,copy_source)):
 obj=w/('extra'+str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;extra.append(obj)
ro,co=extra
mesh_source=ROOT/'rebuild/src/compiled/00/a2/CMeshDataBank_MeshGetters_00a24280.cpp'
mo=w/'mesh.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(mo),str(mesh_source)],env,w);assert not r.returncode,r.stdout+r.stderr
type_source=ROOT/'rebuild/src/compiled/00/be/CLocalDetailObjectCollectionType_Load_00be27b0.cpp'
type_symbol='?Load@CLocalDetailObjectCollectionType@@QAEXAAVCDataInputStream@@_N@Z'
to=w/'type.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(to),str(type_source)],env,w);assert not r.returncode,r.stdout+r.stderr
symbol='?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'_memmove':0xbfeae6,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??_7CLocalDetailObjectCollectionType@@6B@':0x12a7d94,'?Load@CLocalDetailObjectCollectionType@@QAEXAAVCDataInputStream@@_N@Z':0xbe27b0,'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,'?RegisterObjectCollectionType@CEngineLocalDetailGenerator@@QAEPBVCLocalDetailObjectCollectionType@@ABV2@@Z':0xb59c70}
grow=next(s for s in c.symbols if s.startswith('?_M_insert_overflow@'));targets[grow]=0xbdfc00
type_targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'??1CCharString@@QAE@XZ':0x99eae0,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0,'?PeekStaticMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z':0xa24280,'?PeekRepeatedMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z':0xa24290}
targets.update(type_targets)
targets.update({'?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z':0xbe2680,'??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z':0xbe26c0,'??2@YAPAXI@Z':0xbfea1a})
registry_coff=Coff(ro);registry_grow=next(s for s in registry_coff.symbols if s.startswith('?_M_insert_overflow@'));targets[registry_grow]=0xb59f60
boundary={0x993ca0:'@Overflow@16',0x99a2f0:'@BaseCtor@4',0x99a300:'@BaseDtor@4',0x99eae0:'@DestroyString@4',0x9ccdc0:'@Find@12',}
aliases={'@Load@12':symbol,'_TypeTable':'??_7CLocalDetailObjectCollectionType@@6B@'};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'detail_loading_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(1854);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for address,length,offset in [(0xbdedd0,356,0),(0xbe27b0,706,512),(0xa24280,16,1280),(0xa24290,16,1296),(0xb59c70,159,1408),(0xbe26c0,83,1664),(0xbe2680,62,1792)]:
 span=read_virtual_data(raw,address,length);body[offset:offset+length]=span
 for i in md.disasm(span,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   a=i.operands[0].imm;assert a in targets.values();fixups.append((offset+i.address-address+1,a,0))
  for op in i.operands:
   if op.type==3 and op.mem.disp in (0x1436e88,0x1436e98):fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,1))
assert struct.unpack_from('<I',body,248)[0]==0x12a7d94;fixups.append((248,0x12a7d94,1))
assert struct.unpack_from('<I',body,1680)[0]==0x12a7d94;fixups.append((1680,0x12a7d94,1));assert len(fixups)==40
(w/'detail_loading_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
proof=check(o,symbol,0xbdedd0,356,10,targets)
type_proof=check(to,type_symbol,0xbe27b0,706,24,targets)
mesh_proof=[check(mo,s,a,16,0,{}) for s,a in targets.items() if a in (0xa24280,0xa24290)]
# Audit the emitted one-slot virtual table and its real scalar destructor target.
sec,off=c.symbols['??_7CLocalDetailObjectCollectionType@@6B@'];table,rel=c.sections[sec]
assert off==0 and len(table)==4 and rel=={0:('??_ECLocalDetailObjectCollectionType@@UAEPAXI@Z',6)}
scalar=struct.unpack('<I',read_virtual_data(raw,0x12a7d94,4))[0]
scalar_proof=check(o,'??_GCLocalDetailObjectCollectionType@@UAEPAXI@Z',scalar,30,2,dict(targets,**{'??3@YAXPAX@Z':0xbfe9bc}))
registry_proof=check(ro,'?RegisterObjectCollectionType@CEngineLocalDetailGenerator@@QAEPBVCLocalDetailObjectCollectionType@@ABV2@@Z',0xb59c70,159,4,targets)
copy_proof=[check(co,s,a,n,r,targets) for s,a,n,r in [('?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z',0xbe2680,62,0),('??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z',0xbe26c0,83,2)]]
f=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(f),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in (o,to,mo,ro,co):
 r=subprocess.run(cmd+[str(f)]+[str(p) for p in (o,to,mo,ro,co) if p!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(f),str(o),str(to),str(mo),str(ro),str(co)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,retail,count,remaining,position,old,capacity in itertools.product((0,0x95),range(7),(0,1,0x80,0xff),(-1,0,1,3),(0,1,4,5),(0,0xffffffff),(0,2),(2,4)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0x500);put(4,base+0x500+old*4);put(8,base+0x500+capacity*4)
  put(0x204,position);put(0x208,128);put(0x20c,base+0x300);put(0x214,remaining);a[0x300]=retail;put(0x301,count)
  for gen,storage in [(0x400,0x100),(0x440,0x120)]:put(gen+12,base+storage);put(gen+16,base+storage);put(gen+20,base+storage+(0 if old==0 else 16))
  put(0xd10,base+0xb00);put(0xe80,base+0xea0)
  for j in range(4):put(0xea0+j*4,base+0xf00+j*16)
  for j in range(3):put(0xc04+j*32,5)
  cursor=0x305
  for j in range(max(0,count)):
   if fill==0:j=0
   if retail:
    for k in range(3):put(cursor+k*4,(0,1,0xffffffff)[(j+k)%3])
    cursor+=12
   for k in range(13):put(cursor+k*4,0x12340000+j*32+k)
   put(cursor+36,j%3);cursor+=52
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,provider=proof,registry_provider=registry_proof,copy_providers=copy_proof,registry_sha256=hashlib.sha256(registry_source.read_bytes()).hexdigest(),copy_sha256=hashlib.sha256(copy_source.read_bytes()).hexdigest(),mesh_providers=mesh_proof,mesh_sha256=hashlib.sha256(mesh_source.read_bytes()).hexdigest(),type_provider=type_proof,type_sha256=hashlib.sha256(type_source.read_bytes()).hexdigest(),scalar=scalar_proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Five mandatory providers, seven original spans and40 audited fixups. Actual palette/type loading, mesh getters, registration, comparison and copy compose. Full4KiB snapshots; both serialized formats, identical/distinct records, counts/flags, stream splits/wrap, list capacity/growth and callback generator changes. Actual vtable audited. String/stream overflow, name lookup, base lifecycle, allocation and vector growth remain boundaries. Base ctor initializes otherwise uninitialized fields for deterministic comparison.')
(ROOT/'rebuild/compile-gate/native-detail-loading.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_LOADING_GATE_PASS',snapshots,'snapshots')
