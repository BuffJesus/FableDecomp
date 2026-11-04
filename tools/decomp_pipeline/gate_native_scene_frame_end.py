import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-scene-frame-end-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b9'/p for p in ('CEngineScene_FrameEnd_00b94a30.cpp','CEngineScene_UpdateMaps_00b948e0.cpp','CEngineScene_ClearNonPersistent_00b91980.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSceneFrameEnd_test.cpp'
end='?FrameEnd@CEngineScene@@QAEXXZ';update='?UpdateMaps@CEngineScene@@AAEXXZ';clear='?ClearNonPersistent@CEngineScene@@AAEXXZ';child='?Update@CEngineSceneMap@@QAEXXZ'
targets={update:0xb948e0,clear:0xb91980,child:0xb947e0}
aliases={'@EndScene@8':end,child:'@UpdateMap@8'}
(w/'scene_frame_end_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(266);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(end,0xb94a30,16,2,0),(update,0xb948e0,179,1,16),(clear,0xb91980,58,0,208)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e14:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==3
(w/'scene_frame_end_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=11520 snapshots=' in r.stdout
report=dict(cases=11520,providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Native and compiled actual scene FrameEnd, nested map traversal and nonpersistent list cleanup each compared to independent full16KiB model. Balanced/left/right nested trees; zero-to-three columns/maps; zero-to-four primitive nodes; modular reference counts; next-map replacement, head replacement by map callback, head mutation by deletion callback. Map Update and virtual primitive deletion are callbacks. No rendering.')
(ROOT/'rebuild/compile-gate/native-scene-frame-end.json').write_text(json.dumps(report,indent=2)+'\n')

print('NATIVE_SCENE_FRAME_END_GATE_PASS')
