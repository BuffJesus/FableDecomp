import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-frame-scene-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_FrameEnd_00b32820.cpp','00/b9/CEngineScene_FrameEnd_00b94a30.cpp','00/b9/CEngineScene_UpdateMaps_00b948e0.cpp','00/b9/CEngineScene_ClearNonPersistent_00b91980.cpp','00/b9/CEngineScene_ResetStats_00b93df0.cpp','00/b9/SceneStatsTree_Erase_00b95ee0.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveFrameScene_test.cpp'
from gate_game_component_abi import Coff
trial=w/'identity.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(trial),str(sources[5])],env,w);assert not r.returncode,r.stdout+r.stderr
erase=next(s for s in Coff(trial).symbols if s.startswith('?_M_erase'))
root='?FrameEnd@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentParams@@@Z';end='?FrameEnd@CEngineScene@@QAEXXZ';update='?UpdateMaps@CEngineScene@@AAEXXZ';clear='?ClearNonPersistent@CEngineScene@@AAEXXZ';reset='?ResetStats@CEngineScene@@QAEXXZ';child='?Update@CEngineSceneMap@@QAEXXZ'
specs=[(root,0xb32820,158,2,0),(end,0xb94a30,16,2,160),(update,0xb948e0,179,1,176),(clear,0xb91980,58,0,368),(reset,0xb93df0,73,1,432),(erase,0xb95ee0,51,2,512)]
targets={sym:a for sym,a,l,c,o in specs};targets.update({child:0xb947e0,'_free':0xbfea14})
aliases={'@EndFrame@12':root,child:'@MapUpdate@8'}
(w/'primitive_frame_scene_retail.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(563);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e14:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==7
(w/'primitive_frame_scene_retail.h').open('a').write('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
(w/'primitive_frame_scene_retail.h').open('a').write('static const unsigned retailFunctions[][2]={'+','.join('{%d,%d}'%(a,o) for sym,a,l,c,o in specs)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
replay=w/'erase-replay.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Dfree=SceneStatsFree','/Fo'+str(replay),str(sources[5])],env,w);assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[5]),Coff(replay);sec,off=original.symbols[erase];other,otheroff=renamed.symbols[erase];body1,rel1=original.sections[sec];body2,rel2=renamed.sections[other];assert body1==body2 and off==otheroff;assert {o:('_SceneStatsFree' if t=='_free' else t,k) for o,(t,k) in rel1.items()}==rel2;objects[5]=replay
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=2304 snapshots=' in r.stdout
report=dict(cases=2304,providers=proofs,omission_checks=6,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual renderer FrameEnd through second-base virtualslot2, actual scene FrameEnd/map traversal/nonpersistent cleanup/stats reset and tree erase. Full16KiB independent model across scene/manager counts, aliases, mutable array bounds/storage, zero/refcount cases, free scribbling, stats repeat reset. Only scene-map Update, manager FrameEnd, virtual primitive deletion and audited free are boundaries. No visual output.')
(ROOT/'rebuild/compile-gate/native-primitive-frame-scene.json').write_text(json.dumps(report,indent=2)+'\n')

print('NATIVE_PRIMITIVE_FRAME_SCENE_GATE_PASS')
