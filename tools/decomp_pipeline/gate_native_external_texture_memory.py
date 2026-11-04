"""Texture/depth allocation wrappers, mip defaults and callback mutation parity."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-external-texture-memory-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEngineResourceManager_ExternalTexture_00b3b1f0.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp','00/9f/CTexture_Initialise_009fa280.cpp','01/29/CPixelFormat_Table_0129ba40.cpp','00/9f/CTexture_PreallocatedStorage_009fa230.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','00/9e/CPixelFormat_GetColourDepth_009e3820.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeExternalTextureMemory_test.cpp'
allocate='?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z';free='?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z';cleanup='?Uninitialise@CTexture@@QAEXXZ';init='?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z'
targets={'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,init:0x9fa280,cleanup:0x9f9f70}
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';root='?GFGetSystemManager@@YIPAVCSystemManager@@XZ';calc='?CalcByteLength@CTexture@@AAEXXZ'
targets.update({table:0x129ba40,root:0x9a4ec0,calc:0x9f9ee0})
pixel='?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z';depth='?GetColourDepth@CPixelFormat@@QBEKXZ';targets.update({pixel:0x9e3830,depth:0x9e3820})
aliases={'@Allocate@24':allocate,'@Free@12':free,'_FormatTable':table,root:'@GetSystem@0'}
(w/'external_texture_memory_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(589);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(allocate,0xb3b1f0,70,2,0),(free,0xb3b5b0,29,2,80),(cleanup,0x9f9f70,34,0,128),(init,0x9fa280,181,3,176),(calc,0x9f9ee0,143,2,368),(pixel,0x9e3830,50,1,512),(depth,0x9e3820,13,1,576)]
for symbol,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x129ba40,0x129ba48):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
    elif o.type==2 and o.imm in (0x129ba40,0x129ba48):fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
assert len(fixups)==11
(w/'external_texture_memory_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[(0,0,1,2,4,5,6)[n]],symbol,address,length,count,targets) for n,(symbol,address,length,count,offset) in enumerate(specs)]
assert coff_data(Coff(objects[3]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(7):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,index,geometry,status,mode in itertools.product((0x100,0x300),range(46),range(6),(0,1,0x80004005,0xffffffff),range(4)):
  flags=(0,1,0xffffffff,0x12345678)[index%4];levels=(0,1,0xffffffff,0x80000000,7)[(index+geometry)%5];present=(index+geometry+mode)%2
  x,y=((0,0),(7,8),(8,8),(512,256),(0xffffffff,1),(0xffffffff,0xffffffff))[geometry]
  f.write(struct.pack('<9I',base,flags,levels,index,status,mode,present,x,y));f.write(bytes([0x95])*4096);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4096==0;snapshots=size//4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=7,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB independent model with actual resource allocation, texture Initialise, table, cleanup, CalcByteLength, format lookup and colour depth.46formats,sixgeometries,fourHRESULTs,twoobjectplacements,fourcallbackmodes;rotating flags,mipcounts,COMoutput. Root changes dimensions/format/display after mipcount cached; creation mutates packed allocation state. Failed initialization still returns object; subsequent free releases any written COM pointer. Five signed COM mip counts (-1 through3); descriptions return failedHRESULT with populated data. Independent modulo32-bit accounting checks each iteration. Only allocator,root and COM remain callbacks.')
(ROOT/'rebuild/compile-gate/native-external-texture-memory.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_EXTERNAL_TEXTURE_MEMORY_GATE_PASS',snapshots,'snapshots')
