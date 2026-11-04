import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-data-create-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CManagedTextureData_CreateTexture_00a697a0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_data_create_texture_candidate2_20261009.cpp'
symbol='?CreateTexture@CManagedTextureData@@QAE_NKKIABVCPixelFormat@@@Z';dimensions='?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z';init='?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z'
obj=w/'create.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0xa697a0,100,2,{dimensions:0x9fa620,init:0x9fa280});body=read_virtual_data(EXE.read_bytes(),0xa697a0,100);fixups=[];external={0x9fa620:0,0x9fa280:1}
for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(body,0xa697a0):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:target=int(ins.op_str,16);assert target in external;fixups.append((ins.address-0xa697a0+1,external[target],20))
assert len(fixups)==2
(w/'managed_texture_data_create_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'managed_texture_data_create_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ManagedCreate@24='+symbol+'")\n#pragma comment(linker,"/alternatename:'+dimensions+'=@DimensionsBoundary@12")\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureDataCreate_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=16384 snapshots=98304' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_surface_types.h','fable_extents_i.h','fable_counted_pointer.h','fable_resource_list.h','rebuild_abi.h')]
report=dict(cases=16384,snapshots=98304,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete100B exact CManagedTextureData CreateTexture,width-heightulong,third unsignedint unused,constPixelFormatreference. Original next-valid-dimensions andtexture-initializer are functional boundaries; deferred rounding helper not claimed recovered. Returned full32B dimensions copiedtofresh local extent, stored16bit Width/Height,live Texture.Data read after roundingcallback,forcelevels-1/flags0/managedpool1/dynamicfalse/autofalse;storeAllocated andreturn booleanafterinitializercallback.16384cases/98304full16KiB snapshots at bothcalls/final;rawinput/outputdimensions/ignoredlevels/receivers,callbackowner-format-input-outputmutations/two placements-fills. One actual mandatorysource, no extrahelpers/data.')
(ROOT/'rebuild/compile-gate/native-managed-texture-data-create.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_DATA_CREATE_GATE_PASS')
