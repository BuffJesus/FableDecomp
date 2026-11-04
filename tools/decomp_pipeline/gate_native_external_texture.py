"""Texture/depth allocation wrappers, mip defaults and callback mutation parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-external-texture-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEngineResourceManager_ExternalTexture_00b3b1f0.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeExternalTexture_test.cpp'
allocate='?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z';free='?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z';cleanup='?Uninitialise@CTexture@@QAEXXZ';init='?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z'
targets={'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,init:0x9fa280,cleanup:0x9f9f70}
aliases={'@Allocate@24':allocate,'@Free@12':free,init:'@Initialise@36'}
(w/'external_texture_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(162);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(allocate,0xb3b1f0,70,2,0),(free,0xb3b5b0,29,2,80),(cleanup,0x9f9f70,34,0,128)]
for symbol,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
assert len(fixups)==4
(w/'external_texture_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0 if n<2 else 1],symbol,address,length,count,targets) for n,(symbol,address,length,count,offset) in enumerate(specs)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,flags,levels,index,success,mode,present in itertools.product((0x100,0x300),(0,1,0xffffffff,0x12345678),(0,1,0xffffffff,0x80000000,7),range(46),range(2),range(4),range(2)):
  f.write(struct.pack('<7I',base,flags,levels,index,success,mode,present));f.write(bytes([0x95])*1024);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%1024==0;snapshots=size//1024
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full1KiB independent model. Allocation8B, zero initial state, managed pool1/usage0/false booleans; ignores incoming flags and init result. Callback mutates inputs/state. Actual texture cleanup preserves low28 bits and releases COM before delete. Null free included. Allocator always succeeds; Initialise and COM Release are explicit callbacks.')
(ROOT/'rebuild/compile-gate/native-external-texture.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_EXTERNAL_TEXTURE_GATE_PASS',snapshots,'snapshots')
